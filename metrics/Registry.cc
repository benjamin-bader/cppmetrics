#include "Registry.h"

#include "Counter.h"
#include "ExponentiallyDecayingReservoir.h"
#include "Gauge.h"
#include "Meter.h"
#include "Histogram.h"
#include "Timer.h"

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
