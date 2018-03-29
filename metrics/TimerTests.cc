#include "Timer.h"

#include "gtest/gtest.h"

#include "ExponentiallyDecayingReservoir.h"
#include "ManualClock.h"

namespace cppmetrics {

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

  auto result = timer.time([&]() {
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
