#include "WeightedSnapshot.h"

#include <limits>
#include <stdexcept>
#include <utility>

#include "gtest/gtest.h"

namespace cppmetrics {

TEST(WeightedSample, is_pod)
{
  WeightedSample sample{5, 1.0};

  EXPECT_EQ(5, sample.get_value());
  EXPECT_EQ(1.0, sample.get_weight());

  WeightedSample copy(sample);
  EXPECT_EQ(5, copy.get_value());
  EXPECT_EQ(1.0, copy.get_weight());

  WeightedSample moved(std::move(copy));
  EXPECT_EQ(5, moved.get_value());
  EXPECT_EQ(1.0, moved.get_weight());

  WeightedSample assigned = moved;
  EXPECT_EQ(5, assigned.get_value());
  EXPECT_EQ(1.0, assigned.get_weight());
}

TEST(WeightedSample, set_weight)
{
  WeightedSample sample{1, 1.0};
  EXPECT_EQ(1.0, sample.get_weight());

  sample.set_weight(10.0);
  EXPECT_EQ(10.0, sample.get_weight());
}

class WeightedSnapshotTests : public testing::Test
{
protected:
  virtual void SetUp()
  {
    samples = {
      { 5, 1.0 },
      { 1, 2.0 },
      { 2, 3.0 },
      { 3, 2.0 },
      { 4, 2.0 },
    };
  }

  std::vector<WeightedSample> samples;
};

TEST_F(WeightedSnapshotTests, small_quantiles_are_first_value)
{
  WeightedSnapshot snapshot{samples.begin(), samples.end()};
  EXPECT_FLOAT_EQ(1.0, snapshot.get_value(0.0));
}

TEST_F(WeightedSnapshotTests, big_quantiles_are_the_last_value)
{
  WeightedSnapshot snapshot{samples.begin(), samples.end()};
  EXPECT_FLOAT_EQ(5.0, snapshot.get_value(1.0));
}

TEST_F(WeightedSnapshotTests, has_median)
{
  WeightedSnapshot snapshot{samples.begin(), samples.end()};
  EXPECT_FLOAT_EQ(3.0, snapshot.get_median());
}

TEST_F(WeightedSnapshotTests, has_mean)
{
  WeightedSnapshot snapshot{samples.begin(), samples.end()};
  EXPECT_FLOAT_EQ(2.7, snapshot.get_mean());
}

TEST_F(WeightedSnapshotTests, has_values)
{
  WeightedSnapshot snapshot{samples.begin(), samples.end()};
  std::vector<long> expected{1, 2, 3, 4, 5};
  EXPECT_EQ(expected, snapshot.get_values());
}

TEST_F(WeightedSnapshotTests, has_std_dev)
{
  WeightedSnapshot snapshot{samples.begin(), samples.end()};
  EXPECT_FLOAT_EQ(1.2688577, snapshot.get_std_dev());
}

TEST_F(WeightedSnapshotTests, does_not_overflow_for_small_weights)
{
  std::vector<WeightedSample> samples {
    { 1, std::numeric_limits<double>::min() },
    { 2, std::numeric_limits<double>::min() },
    { 3, std::numeric_limits<double>::min() },
  };

  WeightedSnapshot snapshot(std::move(samples));

  EXPECT_FLOAT_EQ(2.0, snapshot.get_mean());
}

TEST_F(WeightedSnapshotTests, throws_on_quantile_less_than_zero)
{
  WeightedSnapshot snapshot{samples.begin(), samples.end()};
  EXPECT_THROW(snapshot.get_value(-1.0), std::domain_error);
}

TEST_F(WeightedSnapshotTests, throws_on_quantile_greater_than_one)
{
  WeightedSnapshot snapshot{samples.begin(), samples.end()};
  EXPECT_THROW(snapshot.get_value(1.01), std::domain_error);
}

}
