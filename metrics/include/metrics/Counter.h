//  Copyright 2018 Benjamin Bader
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

#ifndef CPPMETRICS_METRICS_COUNTER_H
#define CPPMETRICS_METRICS_COUNTER_H

#include <cstdint>
#include <metrics/LongAdder.h>

namespace cppmetrics {

class Counter
{
public:
  using value_t = std::int64_t;

  Counter();
  Counter(const Counter&);
  Counter(Counter&&);

  Counter& operator=(const Counter&);
  Counter& operator=(Counter&&);

  void inc(value_t n = 1);
  void dec(value_t n = 1);

  LongAdder::value_t get_count() const noexcept;

private:
  LongAdder m_adder;
};

} // namespace cppmetrics

#endif
