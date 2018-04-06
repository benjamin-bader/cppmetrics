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

#include <metrics/Timer.h>

#include <utility>

#include <metrics/ExponentiallyDecayingReservoir.h>

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
