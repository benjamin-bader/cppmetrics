#ifndef CPPMETRICS_METRICS_TIMER_H
#define CPPMETRICS_METRICS_TIMER_H

#include <chrono>
#include <memory>

#include "Clock.h"
#include "Histogram.h"
#include "Meter.h"
#include "Reservoir.h"

namespace cppmetrics {

class Snapshot;

class Timer
{
public:
  Timer();
  Timer(std::unique_ptr<Reservoir>&& reservoir);
  Timer(std::unique_ptr<Reservoir>&& reservoir, Clock* clock);
  Timer(Timer&&) = default;

  template <typename Function>
  auto time(Function&& fn) -> decltype(fn());

  void update(const std::chrono::nanoseconds& nanos);
  void update(const std::chrono::milliseconds& millis);

  long get_count();
  double get_m1_rate();
  double get_m5_rate();
  double get_m15_rate();
  double get_mean_rate();
  std::shared_ptr<Snapshot> get_snapshot();

private:
  friend class ScopeTimer;

  Clock* m_clock;
  Histogram m_histogram;
  Meter m_meter;
};

class ScopeTimer
{
public:
  ScopeTimer(Timer& timer)
      : m_timer(timer)
      , m_start(m_timer.m_clock->tick())
  {
  }

  ~ScopeTimer()
  {
    auto elapsed = m_timer.m_clock->tick() - m_start;
    m_timer.update(elapsed);
  }

private:
  Timer& m_timer;
  std::chrono::nanoseconds m_start;
};

template <typename Function>
auto Timer::time(Function&& fn) -> decltype(fn())
{
  ScopeTimer(*this);
  return fn();
}

}

#endif
