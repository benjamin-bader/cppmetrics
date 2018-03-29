#include "EWMA.h"

#include "gtest/gtest.h"

namespace cppmetrics {

inline void elapse_one_minute(EWMA& ewma)
{
  for (int i = 0; i < 12; ++i)
  {
    ewma.tick();
  }
}

TEST(EWMATest, one_minute_ewma_with_value_of_three)
{
  auto ewma = EWMA::one_minute();
  ewma->update(3);
  ewma->tick();

  EXPECT_FLOAT_EQ(0.6, ewma->get_rate(std::chrono::seconds(1)));

  std::vector<double> expected_rates {
    0.22072766,
    0.08120117,
    0.02987224,
    0.01098938,
    0.00404277,
    0.00148725,
    0.00054713,
    0.00020128,
    0.00007405,
    0.00002724,
    0.00001002,
    0.00000369,
    0.00000136,
    0.00000050,
    0.00000018,
  };

  for (auto&& rate : expected_rates)
  {
    elapse_one_minute(*ewma);
    EXPECT_NEAR(rate, ewma->get_rate(std::chrono::seconds(1)), 0.000001);
  }
}

TEST(EWMATest, five_minute_ewma_with_value_of_three)
{
  auto ewma = EWMA::five_minutes();
  ewma->update(3);
  ewma->tick();

  EXPECT_FLOAT_EQ(0.6, ewma->get_rate(std::chrono::seconds(1)));

  std::vector<double> expected_rates {
    0.49123845,
    0.40219203,
    0.32928698,
    0.26959738,
    0.22072766,
    0.18071653,
    0.14795818,
    0.12113791,
    0.09917933,
    0.08120117,
    0.06648190,
    0.05443077,
    0.04456415,
    0.03648604,
    0.02987224,
  };

  for (auto&& rate : expected_rates)
  {
    elapse_one_minute(*ewma);
    EXPECT_NEAR(rate, ewma->get_rate(std::chrono::seconds(1)), 0.000001);
  }
}

TEST(EWMATest, fifteen_minute_ewma_with_value_of_three)
{
  auto ewma = EWMA::fifteen_minutes();
  ewma->update(3);
  ewma->tick();

  EXPECT_FLOAT_EQ(0.6, ewma->get_rate(std::chrono::seconds(1)));

  std::vector<double> expected_rates {
    0.56130419,
    0.52510399,
    0.49123845,
    0.45955700,
    0.42991879,
    0.40219203,
    0.37625345,
    0.35198773,
    0.32928698,
    0.30805027,
    0.28818318,
    0.26959738,
    0.25221023,
    0.23594443,
    0.22072766,
  };

  for (auto&& rate : expected_rates)
  {
    elapse_one_minute(*ewma);
    EXPECT_NEAR(rate, ewma->get_rate(std::chrono::seconds(1)), 0.000001);
  }
}

}
