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


#include <metrics/Registry.h>

#include <metrics/Counter.h>
#include <metrics/ExponentiallyDecayingReservoir.h>
#include <metrics/Gauge.h>
#include <metrics/Meter.h>
#include <metrics/Histogram.h>
#include <metrics/Timer.h>

namespace cppmetrics {

template <typename M>
using MetricPtr = std::shared_ptr<M>;

template <typename M>
using MMap = std::map<std::string, MetricPtr<M>>;

MetricPtr<Gauge> Registry::gauge(const std::string& name)
{
  return get_or_add(name, m_gauges, []() { return std::make_shared<Gauge>(); });
}

MetricPtr<Counter> Registry::counter(const std::string& name)
{
  return get_or_add(name, m_counters, []() { return std::make_shared<Counter>(); });
}

MetricPtr<Meter> Registry::meter(const std::string& name)
{
  return get_or_add(name, m_meters, []() { return std::make_shared<Meter>(); });
}

MetricPtr<Histogram> Registry::histogram(const std::string& name)
{
  return get_or_add(name, m_histograms, []() {
    return std::make_shared<Histogram>(std::make_unique<ExponentiallyDecayingReservoir>());
  });
}

MetricPtr<Timer> Registry::timer(const std::string& name)
{
  return get_or_add(name, m_timers, []() { return std::make_shared<Timer>(); });
}

MMap<Gauge> Registry::get_gauges()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_gauges;
}

MMap<Counter> Registry::get_counters()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_counters;
}

MMap<Meter> Registry::get_meters()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_meters;
}

MMap<Histogram> Registry::get_histograms()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_histograms;
}

MMap<Timer> Registry::get_timers()
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_timers;
}

}
