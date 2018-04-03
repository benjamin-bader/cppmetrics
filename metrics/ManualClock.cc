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

#include "ManualClock.h"

namespace cppmetrics {

ManualClock::ManualClock(const std::chrono::nanoseconds& now)
    : m_now(now)
{}

std::chrono::nanoseconds ManualClock::tick()
{
  return m_now;
}

time_t ManualClock::now_as_time_t()
{
  auto now = std::chrono::duration_cast<std::chrono::system_clock::duration>(m_now);
  std::chrono::system_clock::time_point tp(now);
  return std::chrono::system_clock::to_time_t(tp);
}

void ManualClock::add_nanos(long long nanos)
{
  m_now += std::chrono::nanoseconds(nanos);
}

void ManualClock::add_millis(int millis)
{
  m_now += std::chrono::milliseconds(millis);
}

void ManualClock::add_seconds(int seconds)
{
  m_now += std::chrono::seconds(seconds);
}

void ManualClock::add_minutes(int minutes)
{
  m_now += std::chrono::minutes(minutes);
}

void ManualClock::add_hours(int hours)
{
  m_now += std::chrono::hours(hours);
}

}
