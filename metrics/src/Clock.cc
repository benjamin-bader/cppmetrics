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

#include <metrics/Clock.h>

namespace cppmetrics {

namespace {

Clock* gDefaultClock = new Clock;

}

std::chrono::nanoseconds Clock::tick()
{
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
}

time_t Clock::now_as_time_t()
{
  auto now = std::chrono::system_clock::now();
  return std::chrono::system_clock::to_time_t(now);
}

Clock* GetDefaultClock()
{
  return gDefaultClock;
}

}
