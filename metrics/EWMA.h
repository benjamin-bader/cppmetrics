#ifndef CPPMETRICS_METRICS_EWMA_H
#define CPPMETRICS_METRICS_EWMA_H

#include <atomic>
#include <chrono>
#include <memory>

namespace cppmetrics {

/**
 * A type representing an exponentially-weighted moving average.
 * 
 * NOTE: Expects to be |tick|ed every five seconds.
 */
class EWMA
{
public:
  static std::shared_ptr<EWMA> one_minute();
  static std::shared_ptr<EWMA> five_minutes();
  static std::shared_ptr<EWMA> fifteen_minutes();

  EWMA(double alpha, const std::chrono::system_clock::duration& tick_interval);

  void update(long n);
  void tick();
  double get_rate(const std::chrono::duration<long>& period);

  template <typename Duration>
  const Duration get_tick_interval() const
  {
    return std::chrono::duration_cast<Duration>(m_interval);
  }

private:
  std::atomic_bool m_initialized;
  std::atomic_long m_counter;
  std::chrono::nanoseconds m_interval;
  std::atomic<double> m_rate;
  double m_alpha;
};

}

#endif
