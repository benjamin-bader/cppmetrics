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

#ifndef CPPMETRICS_METRICS_REGISTRY_H
#define CPPMETRICS_METRICS_REGISTRY_H

#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <shared_mutex>
#include <string>

namespace cppmetrics {

class Gauge;
class Counter;
class Meter;
class Histogram;
class Timer;

class Registry
{
public:
  std::shared_ptr<Gauge>     gauge(const std::string& name);
  std::shared_ptr<Counter>   counter(const std::string& name);
  std::shared_ptr<Meter>     meter(const std::string& name);
  std::shared_ptr<Histogram> histogram(const std::string& name);
  std::shared_ptr<Timer>     timer(const std::string& name);

  std::map<std::string, std::shared_ptr<Gauge>>     get_gauges();
  std::map<std::string, std::shared_ptr<Counter>>   get_counters();
  std::map<std::string, std::shared_ptr<Meter>>     get_meters();
  std::map<std::string, std::shared_ptr<Histogram>> get_histograms();
  std::map<std::string, std::shared_ptr<Timer>>     get_timers();

private:
  template <typename T, typename Factory>
  std::shared_ptr<T> get_or_add(
      const std::string& name,
      std::map<std::string, std::shared_ptr<T>>& collection,
      Factory&& factory);

private:
  std::shared_timed_mutex m_mutex;

  std::set<std::string> m_names;
  std::map<std::string, std::shared_ptr<Gauge>>     m_gauges;
  std::map<std::string, std::shared_ptr<Counter>>   m_counters;
  std::map<std::string, std::shared_ptr<Meter>>     m_meters;
  std::map<std::string, std::shared_ptr<Histogram>> m_histograms;
  std::map<std::string, std::shared_ptr<Timer>>     m_timers;
};

template <typename T, typename Factory>
std::shared_ptr<T> Registry::get_or_add(
    const std::string& name,
    std::map<std::string, std::shared_ptr<T>>& collection,
    Factory&& factory)
{
  { // scope for RAII shared-read lock
    std::shared_lock<std::shared_timed_mutex> read_lock(m_mutex);
    if (m_names.find(name) != m_names.end())
    {
      return collection[name];
    }
  }

  std::lock_guard<std::shared_timed_mutex> lock(m_mutex);
  if (m_names.find(name) != m_names.end())
  {
    return collection[name];
  }
  else
  {
    std::shared_ptr<T> metric = factory();
    m_names.insert(name);
    collection[name] = metric;
    return metric;
  }
}

} // namespace cppmetrics

#endif
