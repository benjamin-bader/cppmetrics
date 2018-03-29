#include "ExponentiallyDecayingReservoir.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <cmath>
#include <ctime>
#include <random>
#include <unordered_set>

using namespace std::chrono_literals;

namespace cppmetrics {

namespace {

using namespace std::chrono;

constexpr const std::time_t kRescalePeriod = static_cast<std::time_t>(60); // seconds

const std::time_t NowInSeconds()
{
  std::time_t result;
  return std::time(&result);
}

inline bool HasEquivalentOrder(double lhs, double rhs)
{
  return !(lhs < rhs) && !(rhs < lhs);
}

thread_local std::random_device rd;
thread_local std::uniform_real_distribution<> dist;

} // namespace

constexpr const size_t ExponentiallyDecayingReservoir::kDefaultSize = 1028;
constexpr const double ExponentiallyDecayingReservoir::kDefaultAlpha = 0.99;

ExponentiallyDecayingReservoir::ExponentiallyDecayingReservoir(size_t size, double alpha)
    : m_mutex()
    , m_count(0)
    , m_start(NowInSeconds())
    , m_next_rescale_time(m_start + kRescalePeriod)
    , m_size(size)
    , m_alpha(alpha)
    , m_samples()
{}

ExponentiallyDecayingReservoir::ExponentiallyDecayingReservoir(ExponentiallyDecayingReservoir&& other)
{
  std::lock_guard<std::mutex> lock(other.m_mutex);

  m_count.store(other.m_count.load());
  m_start = other.m_start;
  m_next_rescale_time = other.m_next_rescale_time;
  m_size = other.m_size;
  m_alpha = other.m_alpha;
  m_samples = std::move(other.m_samples);
}

ExponentiallyDecayingReservoir& ExponentiallyDecayingReservoir::operator=(ExponentiallyDecayingReservoir&& other)
{
  std::lock_guard<std::mutex> lock(other.m_mutex);

  m_count.store(other.m_count.load());
  m_start = other.m_start;
  m_next_rescale_time = other.m_next_rescale_time;
  m_size = other.m_size;
  m_alpha = other.m_alpha;
  m_samples = std::move(other.m_samples);

  return *this;
}

size_t ExponentiallyDecayingReservoir::size() const
{
  return std::min(m_size, static_cast<size_t>(m_count.load()));
}

void ExponentiallyDecayingReservoir::update(long value)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  auto now = NowInSeconds();
  auto scale_factor = now - m_start;
  double item_weight = std::exp(m_alpha * scale_factor);
  double priority = item_weight / dist(rd);
  
  auto new_size = m_count.fetch_add(1) + 1;
  if (new_size <= m_size)
  {
    m_samples[priority] = WeightedSample{value, item_weight};
  }
  else
  {
    const auto& smallest_pair = *m_samples.begin();
    double smallest_priority = smallest_pair.first;
    if (smallest_priority < priority)
    {
      auto insert_result = m_samples.insert(std::make_pair(priority, WeightedSample{value, item_weight}));
      bool did_insert = insert_result.second;
      if (did_insert)
      {
        m_samples.erase(smallest_priority);
      }
    }
  }
}

std::shared_ptr<Snapshot> ExponentiallyDecayingReservoir::get_snapshot()
{
  rescale_if_needed();

  std::lock_guard<std::mutex> lock(m_mutex);

  std::vector<WeightedSample> samples;
  samples.reserve(m_samples.size());
  std::transform(
      std::begin(m_samples), std::end(m_samples), std::back_inserter(samples),
      [](auto&& pair) { return pair.second; }
  );
  return std::make_shared<WeightedSnapshot>(std::move(samples));
}

void ExponentiallyDecayingReservoir::rescale_if_needed()
{
  auto now = NowInSeconds();
  if (now >= m_next_rescale_time)
  {
    rescale(now, m_next_rescale_time);
  }
}

void ExponentiallyDecayingReservoir::rescale(std::time_t now, std::time_t next)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  m_next_rescale_time = now + kRescalePeriod;
  const auto old_start_time = m_start;
  m_start = NowInSeconds();

  const double scaling_factor = exp(-m_alpha * (m_start - old_start_time));
  if (HasEquivalentOrder(scaling_factor, 0.0))
  {
    m_samples.clear();
  }
  else
  {
    std::unordered_set<double> keys;
    keys.reserve(m_samples.size());
    for (auto&& pair : m_samples)
    {
      keys.insert(pair.first);
    }

    for (auto&& key : keys)
    {
      auto it = m_samples.find(key);
      assert(it != m_samples.end());
      auto value = it->second.get_value();
      auto new_weight = it->second.get_weight() * scaling_factor;
      m_samples.erase(it);

      if (!HasEquivalentOrder(new_weight, 0))
      {
        m_samples[key * scaling_factor] = {value, new_weight};
      }
    }
  }

  m_count.store(m_samples.size());
}

} // namespace cppmetrics
