#include "Timer.h"

#include <utility>

#include "ExponentiallyDecayingReservoir.h"

namespace cppmetrics {

Timer::Timer()
    : Timer(std::make_unique<ExponentiallyDecayingReservoir>())
{}

Timer::Timer(std::unique_ptr<Reservoir>&& reservoir)
    : Timer(std::move(reservoir), GetDefaultClock())
{}

Timer::Timer(std::unique_ptr<Reservoir>&& reservoir, Clock* clock)
    : m_clock(clock)
    , m_histogram(std::move(reservoir))
    , m_meter()
{}

void Timer::update(const std::chrono::nanoseconds& nanos)
{
  if (nanos.count() >= 0)
  {
    m_meter.mark();
    m_histogram.update(nanos.count());
  }
}

void Timer::update(const std::chrono::milliseconds& millis)
{
  update(std::chrono::duration_cast<std::chrono::nanoseconds>(millis));
}

long Timer::get_count()
{
  return m_meter.get_count();
}

double Timer::get_m1_rate()
{
  return m_meter.get_m1_rate();
}
double Timer::get_m5_rate()
{
  return m_meter.get_m5_rate();
}

double Timer::get_m15_rate()
{
  return m_meter.get_m15_rate();
}

double Timer::get_mean_rate()
{
  return m_meter.get_mean_rate();
}

std::shared_ptr<Snapshot> Timer::get_snapshot()
{
  return m_histogram.get_snapshot();
}

} // namespace cppmetrics
