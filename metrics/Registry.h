#ifndef CPPMETRICS_METRICS_REGISTRY_H
#define CPPMETRICS_METRICS_REGISTRY_H

#include <map>
#include <memory>
#include <mutex>
#include <set>
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
  std::mutex m_mutex;

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
  std::lock_guard<std::mutex> lock(m_mutex);
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
