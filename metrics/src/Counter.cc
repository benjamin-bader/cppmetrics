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

#include "metrics/Counter.h"

namespace cppmetrics {

Counter::Counter()
    : m_count(0)
{}

Counter::Counter(const Counter& other)
    : m_count(other.get_count())
{}

Counter::Counter(Counter&& other)
    : m_count(other.m_count.exchange(0))
{}

Counter& Counter::operator=(const Counter& other)
{
  m_count = other.get_count();
  return *this;
}

Counter& Counter::operator=(Counter&& other)
{
  m_count = other.m_count.exchange(0);
  return *this;
}

void Counter::inc(long n)
{
  m_count += n;
}

void Counter::dec(long n)
{
  m_count -= n;
}

long Counter::get_count() const noexcept
{
  return m_count.load();
}

}
