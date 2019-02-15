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

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

namespace cppmetrics {

constexpr const std::size_t numIters = 1000000;
constexpr const std::size_t numThreads = 24;
constexpr const std::size_t expectedCount = numIters * numThreads;

}

#ifndef BENCH

#include "gtest/gtest.h"

#include "ManualClock.h"

namespace cppmetrics {

TEST(LongAdderTest, Uncontended)
{
  LongAdder adder;

  adder.incr();
  adder.incr();
  adder.incr();

  EXPECT_EQ(3, adder.count());
}

TEST(LongAdderTest, SeveralThreads)
{
  LongAdder adder;

  auto start = std::chrono::steady_clock::now();

  std::vector<std::thread> threads;
  threads.reserve(numThreads);
  for (int i = 0; i < numThreads; ++i)
  {
    threads.emplace_back([&]() {
      for (int i = 0; i < numIters; ++i)
      {
        adder.incr();
      }
    });
  }

  for (auto&& t : threads)
  {
    t.join();
  }

  auto end = std::chrono::steady_clock::now();
  auto duration = end - start;
  std::cerr << "Finished in " << std::chrono::duration<double, std::milli>(duration).count() << " ms" << std::endl;

  EXPECT_EQ(expectedCount, adder.count());
}

TEST(LongAdderTest, SeveralThreadsWithOneAtomic)
{
  std::atomic_size_t adder{0};

  auto start = std::chrono::steady_clock::now();

  std::vector<std::thread> threads;
  threads.reserve(numThreads);
  for (int i = 0; i < numThreads; ++i)
  {
    threads.emplace_back([&]() {
      for (int i = 0; i < numIters; ++i)
      {
        adder.fetch_add(1);
      }
    });
  }

  for (auto&& t : threads)
  {
    t.join();
  }

  auto end = std::chrono::steady_clock::now();
  auto duration = end - start;
  std::cerr << "Finished in " << std::chrono::duration<double, std::milli>(duration).count() << " ms" << std::endl;

  EXPECT_EQ(expectedCount, adder.load());
}

}

#else

int main(int argc, char** argv)
{
  using namespace cppmetrics;

  LongAdder adder;
  std::atomic_size_t ast{0};

  auto start = std::chrono::steady_clock::now();

  std::vector<std::thread> threads;
  threads.reserve(numThreads);
  for (int i = 0; i < numThreads; ++i)
  {
    threads.emplace_back([&]() {
      for (int i = 0; i < numIters; ++i)
      {
        adder.incr();
        //ast.fetch_add(1);
      }
    });
  }

  for (auto&& t : threads)
  {
    t.join();
  }

  auto end = std::chrono::steady_clock::now();
  auto duration = end - start;
  std::cerr << "Finished in " << std::chrono::duration<double, std::milli>(duration).count() << " ms" << std::endl;

  std::cerr << "Final count: " << adder.count() << ", expected: " << (numThreads * numIters) << std::endl;

  return 0;
}

#endif
