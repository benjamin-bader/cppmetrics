#include "Meter.h"

#include "gtest/gtest.h"

#include "ManualClock.h"

namespace cppmetrics {

TEST(MeterTests, starts_with_zero)
{
  ManualClock clock;
  Meter meter{&clock};
  EXPECT_NEAR(0.0, meter.get_m1_rate(),   0.001);
  EXPECT_NEAR(0.0, meter.get_m5_rate(),   0.001);
  EXPECT_NEAR(0.0, meter.get_m15_rate(),  0.001);
  EXPECT_NEAR(0.0, meter.get_mean_rate(), 0.001);

  EXPECT_EQ(0, meter.get_count());
}

TEST(MeterTests, mark_updates_rates_and_counts)
{
  ManualClock clock;
  Meter meter{&clock};

  meter.mark();
  clock.add_seconds(10); // two ticks
  meter.mark(2);

  EXPECT_NEAR(0.3, meter.get_mean_rate(), 0.001);
  EXPECT_NEAR(0.1840, meter.get_m1_rate(), 0.001);
  EXPECT_NEAR(0.1966, meter.get_m5_rate(), 0.001);
  EXPECT_NEAR(0.1988, meter.get_m15_rate(), 0.001);
}

}
