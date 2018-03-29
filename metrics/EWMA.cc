#include "EWMA.h"

#include <cmath>

namespace cppmetrics {

namespace {

const int kTickIntervalInSeconds = 5;
const std::chrono::system_clock::duration kTickInterval = std::chrono::seconds{kTickIntervalInSeconds};

const double kAlphaM1  = 1 - exp(-kTickIntervalInSeconds / 60.0 / 1);
const double kAlphaM5  = 1 - exp(-kTickIntervalInSeconds / 60.0 / 5);
const double kAlphaM15 = 1 - exp(-kTickIntervalInSeconds / 60.0 / 15);

}

std::shared_ptr<EWMA> EWMA::one_minute()
{
  return std::make_shared<EWMA>(kAlphaM1, kTickInterval);
}

std::shared_ptr<EWMA> EWMA::five_minutes()
{
  return std::make_shared<EWMA>(kAlphaM5, kTickInterval);
}

std::shared_ptr<EWMA> EWMA::fifteen_minutes()
{
  return std::make_shared<EWMA>(kAlphaM15, kTickInterval);
}

EWMA::EWMA(double alpha, const std::chrono::system_clock::duration& tick_interval)
    : m_initialized(false)
    , m_counter(0)
    , m_interval(std::chrono::duration_cast<std::chrono::nanoseconds>(tick_interval))
    , m_rate(0.0)
    , m_alpha(alpha)
{}

void EWMA::update(long n)
{
  m_counter += n;
}

void EWMA::tick()
{
  long count = m_counter.exchange(0);
  double instant_rate = static_cast<double>(count) / m_interval.count();
  if (m_initialized.load())
  {
    double old_rate = m_rate.load();
    m_rate = old_rate + (m_alpha * (instant_rate - old_rate));
  }
  else
  {
    m_rate = instant_rate;
    m_initialized = true;
  }
}

double EWMA::get_rate(const std::chrono::duration<long>& period)
{
  return m_rate * std::chrono::duration_cast<std::chrono::nanoseconds>(period).count();
} 

} // namespace cppmetrics
