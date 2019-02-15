//  Copyright 2019 Benjamin Bader
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <metrics/LongAdder.h>

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <thread>
#include <type_traits>
#include <vector>

#include "AlignedAllocations.h"


namespace cppmetrics {
namespace {

namespace Thread {

std::uint64_t id(bool rehash)
{
  static std::atomic<std::uint64_t> threadIdAllocator {1};
  thread_local static std::uint64_t my_id = threadIdAllocator.fetch_add(1);
  if (rehash)
  {
    // std::hash is notably slower than this routine, which is
    // also lifted from Striped64.java.
    my_id ^= my_id << 13;
    my_id ^= my_id >> 17;
    my_id ^= my_id >> 5;
  }
  return my_id;
}

} // namespace <anon>::Thread

class SpinUnlocker
{
public:
  SpinUnlocker(std::atomic_bool& lock)
    : m_lock(lock)
  {}

  ~SpinUnlocker()
  {
    bool expected = true;
    m_lock.compare_exchange_strong(expected, false, std::memory_order_seq_cst);
  }

private:
  std::atomic_bool& m_lock;
};

constexpr std::size_t next_power_of_two(std::size_t n)
{
  std::size_t result = 1;
  while (result <= n)
  {
    result <<= 1;
  }
  return result;
}

} // namespace

constexpr const std::size_t kCacheLine = 128; // True on all of our target platforms, AFAICT.

/**
 * An atomic counter that is padded to the size of a typical cache line,
 * which reduces the large performance penalty associated with false sharing.
 * 
 * Note that heap-allocated [Cell] instances *must* be aligned to 64 bytes,
 * which (in C++14) requires manual work.  Use [AlignedAllocations::Allocate],
 * or do it manually (e.g. via malloc, computing the nearest aligned address),
 * and use placement-new.
 */
class alignas(kCacheLine) LongAdder::Cell
{
public:
  Cell(std::size_t initial)
    : m_atomic(initial)
  {}

  ~Cell() = default;

  value_t value() const
  {
    return m_atomic.load(std::memory_order_acquire);
  }

  bool cas(value_t expected, value_t replacement)
  {
    return m_atomic.compare_exchange_strong(expected, replacement, std::memory_order_release);
  }

private:
  std::atomic<LongAdder::value_t> m_atomic;
};

LongAdder::LongAdder()
  : m_base_count(0)
  , m_spinlock(0)
  , m_cells()
{
  std::size_t tableSize = next_power_of_two(std::thread::hardware_concurrency());
  m_cells.resize(tableSize, nullptr);
}

LongAdder::~LongAdder()
{
  for (auto&& pCell : m_cells)
  {
    if (pCell != nullptr)
    {
      AlignedAllocations::Free(pCell);
    }
  }
}

void LongAdder::incr(value_t n)
{
  modify(n);
}

void LongAdder::decr(value_t n)
{
  modify(-n);
}

LongAdder::value_t LongAdder::count() const noexcept
{
  value_t base = m_base_count.load(std::memory_order_acquire);

  for (auto&& cell : m_cells)
  {
    if (cell != nullptr)
    {
      base += cell->value();
    }
  }

  return base;
}

void LongAdder::modify(value_t n)
{
  bool collide = false;
  uint64_t h = Thread::id(false);
  while (true)
  {
    std::size_t tableSize = m_cells.size();
    std::size_t mask = (tableSize - 1);
    std::size_t index = h & mask;
    Cell* cell = m_cells[index];

    if ((cell = m_cells[index]) == nullptr)
    {
      if (!is_locked())
      {
        bool created = false;
        cell = AlignedAllocations::Make<Cell>(n);

        if (!is_locked() && lock())
        {
          SpinUnlocker unlocker(m_spinlock);
          if (m_cells[index] == nullptr)
          {
            created = true;
            m_cells[index] = cell;
          }

          if (created)
          {
            break;
          }
          else
          {
            AlignedAllocations::Free(cell);
            continue;
          }
        }

        AlignedAllocations::Free(cell);
      }
      collide = false;
    }
    else
    {
      value_t expected = cell->value();
      if (cell->cas(expected, expected + n))
      {
        // woot
        break;
      }
      else if (!collide)
      {
        collide = true;;
        h = Thread::id(true);
      }
      else
      {
        expected = m_base_count.load(std::memory_order_acquire);
        if (m_base_count.compare_exchange_strong(expected, expected + n, std::memory_order_release))
        {
          break;
        }
      }
    }
  }
}

bool LongAdder::is_locked()
{
  return m_spinlock.load();
}

bool LongAdder::lock()
{
  bool expected = false;
  return m_spinlock.compare_exchange_strong(expected, true);
}

} // cppmetrics
