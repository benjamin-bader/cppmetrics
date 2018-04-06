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

#include <metrics/Timer.h>

#include <chrono>

#include "gtest/gtest.h"

#include <metrics/ExponentiallyDecayingReservoir.h>
#include <metrics/Clock.h>

#include "ManualClock.h"

namespace cppmetrics {

class SelfWindingClock : public Clock
{
public:
  std::chrono::nanoseconds tick() override
  {
    return m_now += std::chrono::milliseconds(50);
  }

private:
  std::chrono::nanoseconds m_now;
};

TEST(TimerTests, starts_with_zero_rates_and_count)
{
  Timer timer;
  EXPECT_EQ(0, timer.get_count());

  EXPECT_EQ(0, timer.get_m1_rate());
  EXPECT_EQ(0, timer.get_m5_rate());
  EXPECT_EQ(0, timer.get_m15_rate());
  EXPECT_EQ(0, timer.get_mean_rate());
}

TEST(TimerTests, updates_count_on_update)
{
  Timer timer;

  EXPECT_EQ(0, timer.get_count());
  timer.update(std::chrono::milliseconds(1));
  EXPECT_EQ(1, timer.get_count());
}

TEST(TimerTests, times_scopes)
{
  ManualClock clock;
  Timer timer(std::make_unique<ExponentiallyDecayingReservoir>(), &clock);

  {
    ScopeTimer scope(timer);
    clock.add_seconds(5);
  }

  EXPECT_EQ(1, timer.get_count());
}

TEST(TimerTests, times_callables)
{
  ManualClock clock;
  Timer timer(std::make_unique<ExponentiallyDecayingReservoir>(), &clock);

  std::string result = timer.time([&]() {
    clock.add_minutes(1);
    return "foo";
  });

  EXPECT_EQ("foo", result);
  EXPECT_EQ(1, timer.get_count());
}

TEST(TimerTests, times_runnables)
{
  ManualClock clock;
  Timer timer(std::make_unique<ExponentiallyDecayingReservoir>(), &clock);

  timer.time([&]() {
    clock.add_minutes(1);
  });

  EXPECT_EQ(1, timer.get_count());
}

}
