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
