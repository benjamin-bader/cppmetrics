#include "ExponentiallyDecayingReservoir.h"

#include "gtest/gtest.h"

namespace cppmetrics {

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

}
