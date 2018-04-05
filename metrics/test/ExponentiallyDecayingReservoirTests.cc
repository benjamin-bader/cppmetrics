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

#include "metrics/ExponentiallyDecayingReservoir.h"

#include "gtest/gtest.h"

#include "ManualClock.h"

namespace cppmetrics {

inline void AssertSnapshotValuesBetween(const std::shared_ptr<Snapshot>& snapshot, long min, long max)
{
  for (long value : snapshot->get_values())
  {
    EXPECT_LE(min, value);
    EXPECT_GT(max, value);
  }
}

TEST(EDRTest, sample_100_of_1000)
{
  ExponentiallyDecayingReservoir reservoir(100, 0.99);

  for (int i = 0; i < 1000; ++i)
  {
    reservoir.update(i);
  }

  EXPECT_EQ(100, reservoir.size());

  auto snapshot = reservoir.get_snapshot();
  EXPECT_EQ(100, snapshot->size());

  for (long value : snapshot->get_values())
  {
    EXPECT_LE(0, value);
    EXPECT_GT(1000, value);
  }
}

TEST(EDRTest, long_inactivity_does_not_corrupt_sampling_state)
{
  ManualClock clock;
  ExponentiallyDecayingReservoir reservoir(10, 0.015, &clock);

  // Add 1000 values at the rate of 10 values per second
  for (int i = 0; i < 1000; ++i)
  {
    reservoir.update(1000 + i);
    clock.add_millis(100);
  }

  EXPECT_EQ(10, reservoir.get_snapshot()->size());
  AssertSnapshotValuesBetween(reservoir.get_snapshot(), 1000, 2000);

  // Wait 15 hours, then add another value.
  // This should trigger a rescale, which will reduce
  // the number of samples to one; the rest will be zeroed
  // out and removed due to extremely small weights.
  clock.add_hours(15);
  reservoir.update(2000);
  EXPECT_EQ(1, reservoir.get_snapshot()->size());
  AssertSnapshotValuesBetween(reservoir.get_snapshot(), 1000, 3000);

  // Add 1000 more values at 10 values/sec
  for (int i = 0; i < 1000; ++i)
  {
    reservoir.update(3000 + i);
    clock.add_millis(100);
  }

  auto snapshot = reservoir.get_snapshot();
  EXPECT_EQ(10, snapshot->size());
  AssertSnapshotValuesBetween(snapshot, 3000, 4000);
}

TEST(EDRTest, snapshotting_after_long_inactivity_rescales)
{
  ManualClock clock;
  ExponentiallyDecayingReservoir reservoir(10, 0.015, &clock);

  // Add 1000 values at the rate of 10 values per second
  for (int i = 0; i < 1000; ++i)
  {
    reservoir.update(1000 + i);
    clock.add_millis(100);
  }

  EXPECT_EQ(10, reservoir.get_snapshot()->size());
  AssertSnapshotValuesBetween(reservoir.get_snapshot(), 1000, 2000);

  // Wait 20 hours (some arbitrary amount of time long enough to zero out all samples)
  clock.add_hours(20);

  auto snapshot = reservoir.get_snapshot();
  EXPECT_EQ(0, snapshot->get_max());
  EXPECT_EQ(0, snapshot->get_mean());
  EXPECT_EQ(0, snapshot->get_median());
  EXPECT_EQ(0, snapshot->get_min());
  EXPECT_EQ(0, snapshot->size());
}

TEST(EDRTest, spot_lift)
{
  ManualClock clock;
  ExponentiallyDecayingReservoir reservoir(1000, 0.015, &clock);

  const int values_rpm = 10;
  const int values_interval_millis = (60 * 1000) / values_rpm;

  // mode 1: steady values for 120 minutes
  for (int i = 0; i < 120 * values_rpm; ++i)
  {
    reservoir.update(177);
    clock.add_millis(values_interval_millis);
  }

  // mode 2: 10 more minutes, at same rate, but higher values
  for (int i = 0; i < 10 * values_rpm; ++i)
  {
    reservoir.update(9999);
    clock.add_millis(values_interval_millis);
  }

  // Mode 2 should predominate after 10 minutes.
  auto snapshot = reservoir.get_snapshot();
  EXPECT_EQ(9999, snapshot->get_median());
}

TEST(EDRTest, spot_fall)
{
  ManualClock clock;
  ExponentiallyDecayingReservoir reservoir(1000, 0.015, &clock);

  const int values_rpm = 10;
  const int values_interval_millis = (60 * 1000) / values_rpm;

  // mode 1: steady values for 120 minutes
  for (int i = 0; i < 120 * values_rpm; ++i)
  {
    reservoir.update(9998);
    clock.add_millis(values_interval_millis);
  }

  // mode 2: 10 more minutes, at same rate, but smaller values
  for (int i = 0; i < 10 * values_rpm; ++i)
  {
    reservoir.update(178);
    clock.add_millis(values_interval_millis);
  }

  // Mode 2 should predominate after 10 minutes.
  auto snapshot = reservoir.get_snapshot();
  EXPECT_EQ(178, snapshot->get_median());
}

TEST(EDRTest, quantiles_are_based_on_weights)
{
  ManualClock clock;
  ExponentiallyDecayingReservoir reservoir(1000, 0.015, &clock);

  for (int i = 0; i < 40; ++i)
  {
    reservoir.update(177);
  }

  clock.add_minutes(2);

  for (int i = 0; i < 10; ++i)
  {
    reservoir.update(9999);
  }

  // The first 40 items have a weight of 1
  // The next 10 have a weight of approximately 6.
  // The distribution is 40 vs 60, not 40 vs 10.
  auto snapshot = reservoir.get_snapshot();
  EXPECT_EQ(50, snapshot->size());

  EXPECT_EQ(177,  snapshot->get_value(0.35));
  EXPECT_EQ(9999, snapshot->get_median());
  EXPECT_EQ(9999, snapshot->get_p75());
}

}
