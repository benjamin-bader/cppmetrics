#include "ManualClock.h"

namespace cppmetrics {

ManualClock::ManualClock(const std::chrono::nanoseconds& now)
    : m_now(now)
{}

std::chrono::nanoseconds ManualClock::tick()
{
  return m_now;
}

void ManualClock::add_nanos(long long nanos)
{
  m_now += std::chrono::nanoseconds(nanos);
}

void ManualClock::add_seconds(int seconds)
{
  m_now += std::chrono::seconds(seconds);
}

void ManualClock::add_minutes(int minutes)
{
  m_now += std::chrono::minutes(minutes);
}

void ManualClock::add_hours(int hours)
{
  m_now += std::chrono::hours(hours);
}

}
