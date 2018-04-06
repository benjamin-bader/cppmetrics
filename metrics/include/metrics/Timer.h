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

#ifndef CPPMETRICS_METRICS_TIMER_H
#define CPPMETRICS_METRICS_TIMER_H

#include <chrono>
#include <memory>

#include <metrics/Clock.h>
#include <metrics/Histogram.h>
#include <metrics/Meter.h>
#include <metrics/Reservoir.h>

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
