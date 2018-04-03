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

#include "Meter.h"

#include <iostream>

#include "Clock.h"
#include "EWMA.h"

namespace cppmetrics {

namespace {

using namespace std::chrono;
using namespace std::chrono_literals;

const long long kTickInterval = duration_cast<nanoseconds>(5s).count();

}

Meter::Meter(Clock* clock)
    : m_clock(clock != nullptr ? clock : GetDefaultClock())
    , m_count(0)
    , m_start_time(m_clock->tick())
    , m_last_tick(m_start_time.count())
    , m_m1( EWMA::one_minute() )
    , m_m5( EWMA::five_minutes() )
    , m_m15( EWMA::fifteen_minutes() )
{}

void Meter::mark(long n)
{
  tick_if_necessary();
  m_count += n;
  m_m1->update(n);
  m_m5->update(n);
  m_m15->update(n);
}

void Meter::tick_if_necessary()
{
  auto old_tick = m_last_tick.load();
  auto new_tick = m_clock->tick().count();
  auto age = new_tick - old_tick;
  if (age > kTickInterval)
  {
    auto new_interval_start_tick = new_tick - age % kTickInterval;
    if (std::atomic_compare_exchange_strong(&m_last_tick, &old_tick, new_interval_start_tick))
    {
      auto required_ticks = age / kTickInterval;
      std::cerr << "Advancing meter by " << required_ticks << " ticks" << std::endl;
      for (int i = 0; i < required_ticks; ++i)
      {
        m_m1->tick();
        m_m5->tick();
        m_m15->tick();
      }
    }
  }
}

long Meter::get_count() const noexcept
{
  return m_count.load();
}

double Meter::get_m1_rate()
{
  tick_if_necessary();
  return m_m1->get_rate(std::chrono::seconds(1));
}

double Meter::get_m5_rate()
{
  tick_if_necessary();
  return m_m5->get_rate(std::chrono::seconds(1));
}

double Meter::get_m15_rate()
{
  tick_if_necessary();
  return m_m15->get_rate(std::chrono::seconds(1));
}

double Meter::get_mean_rate()
{
  auto count = get_count();
  if (count == 0)
  {
    return 0.0;
  }

  constexpr auto kNanosPerSecond = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1)).count();
  auto elapsed = m_clock->tick() - m_start_time;
  return static_cast<double>(count) / elapsed.count() * kNanosPerSecond;
}

}
