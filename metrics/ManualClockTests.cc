#include "ManualClock.h"

#include <chrono>

#include "gtest/gtest.h"

namespace cppmetrics {

TEST(ManualClockTests, equivalent_to_system_clock)
{
  auto now = std::chrono::system_clock::now();
  auto now_as_epoch = std::chrono::system_clock::to_time_t(now);

  ManualClock clock{std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch())};

  EXPECT_EQ(now_as_epoch, clock.now_as_time_t());
}

}
