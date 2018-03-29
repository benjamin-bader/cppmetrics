#ifndef CPPMETRICS_METRICS_METER_H
#define CPPMETRICS_METRICS_METER_H

#include <atomic>
#include <chrono>
#include <memory>

namespace cppmetrics {

class Clock;
class EWMA;

class Meter
{
public:
  Meter(Clock* clock = nullptr);

  void mark(long n = 1);

  long get_count() const noexcept;
  double get_m15_rate();
  double get_m5_rate();
  double get_m1_rate();
  double get_mean_rate();

private:
  void tick_if_necessary();

private:
  Clock* m_clock;

  std::atomic_long m_count;
  std::chrono::nanoseconds m_start_time;
  std::atomic_llong m_last_tick;

  std::shared_ptr<EWMA> m_m1;
  std::shared_ptr<EWMA> m_m5;
  std::shared_ptr<EWMA> m_m15;
};

}

#endif
