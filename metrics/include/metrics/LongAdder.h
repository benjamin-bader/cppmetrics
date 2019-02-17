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

#ifndef CPPMETRICS_METRICS_LONGADDER_H
#define CPPMETRICS_METRICS_LONGADDER_H

#include <atomic>
#include <cstdint>
#include <vector>

namespace cppmetrics {

/**
 * A more-or-less port of java.util.concurrent.LongAdder.
 *
 * A LongAdder is a variant of an atomic counter that it optimized for low-latency
 * updates under high contention.  It maintains a list of cells that functions as
 * a kind of hash table from thread IDs to counters.  A thread-local ID is maintained
 * for each thread, mapping it to a single cell.  As threads contend with each other
 * for access, their ID will be re-hashed.  Assuming CPU-bound threads, eventually all
 * threads will stabilize on a contention-free configuration.  Software isn't usually
 * written with threads bound to individual cores, but nevertheless this algorithm
 * has been shown to be highly performant in the real world via the JDK's implementation.
 *
 * Fetching the value of the adder is not an atomic operation!  If updates are made while
 * the value is being computed, they are not guaranteed to be included in the result.
 */
class LongAdder
{
public:
  using value_t = std::int64_t;

  LongAdder();
  ~LongAdder();

  void incr(value_t = 1);
  void decr(value_t = 1);

  value_t count() const noexcept;

private:
  //void modify(std::function<std::size_t(std::size_t)>&& fn);
  void modify(value_t n);

  bool is_locked();
  bool lock();

private:
  class Cell;

  std::atomic<value_t> m_base_count;
  std::atomic_bool m_spinlock;
  std::vector<Cell*> m_cells;

  // FIXME, maybe
  // We currently don't have a good way to emulate Striped64's method of
  // growing the cell list as needed without either losing updates, or leaking
  // memory.  Suffice to say that we would require proper support for atomic
  // shared_ptr<T>.
  //
  // Currently, we just allocate the table to its maximum size.
};

}

#endif // CPPMETRICS_METRICS_LONGADDER_H
