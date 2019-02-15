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

#include <metrics/Counter.h>

namespace cppmetrics {

Counter::Counter()
    : m_adder()
{}

Counter::Counter(const Counter& other)
    : m_adder()
{
  m_adder.incr(other.get_count());
}

Counter::Counter(Counter&& other)
    : m_adder()
{
  m_adder.incr(other.m_adder.count());
  other.m_adder.decr(other.m_adder.count());
}

Counter& Counter::operator=(const Counter& other)
{
  m_adder.decr(m_adder.count());
  m_adder.incr(other.get_count());
  return *this;
}

Counter& Counter::operator=(Counter&& other)
{
  m_adder.decr(m_adder.count());
  m_adder.incr(other.m_adder.count());
  other.m_adder.decr(m_adder.count());
  return *this;
}

void Counter::inc(value_t n)
{
  m_adder.incr(n);
}

void Counter::dec(value_t n)
{
  m_adder.decr(n);
}

Counter::value_t Counter::get_count() const noexcept
{
  return m_adder.count();
}

}
