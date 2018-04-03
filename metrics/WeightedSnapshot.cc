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

#include "WeightedSnapshot.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace cppmetrics {

namespace {

inline bool HasEquivalentOrder(double lhs, double rhs)
{
  if (isnan(lhs))
  {
    return isnan(rhs);
  }

  return !isnan(rhs) && !(lhs < rhs) && !(rhs < lhs);
}

}

WeightedSample::WeightedSample() : WeightedSample(0, 0.0) {}
WeightedSample::WeightedSample(long value, double weight)
    : m_value(value)
    , m_weight(weight)
{}

WeightedSample::WeightedSample(const WeightedSample& that) = default;
WeightedSample::WeightedSample(WeightedSample&& other) = default;

WeightedSample& WeightedSample::operator=(const WeightedSample&) = default;
WeightedSample& WeightedSample::operator=(WeightedSample&&) = default;

//////////////

WeightedSnapshot::WeightedSnapshot(std::vector<WeightedSample>&& samples)
    : m_elements()
{
  std::sort(
    std::begin(samples),
    std::end(samples),
    [](auto&& lhs, auto&& rhs) { return lhs.get_value() < rhs.get_value(); });

  // zero-initialize vector of elements to the expected size.
  // Note 'resize', not 'reserve'.
  m_elements.resize(samples.size());

  double sum_weight = 0;
  for (auto&& sample : samples)
  {
    sum_weight += sample.get_weight();
  }

  bool zero_weight = HasEquivalentOrder(sum_weight, 0);

  for (size_t ix = 0; ix < samples.size(); ++ix)
  {
    m_elements[ix].value = samples[ix].get_value();
    m_elements[ix].norm_weight = zero_weight ? 0.0 : samples[ix].get_weight() / sum_weight;
    m_elements[ix].quantile = 0.0;
  }

  for (size_t ix = 1; ix < m_elements.size(); ++ix)
  {
    m_elements[ix].quantile = m_elements[ix - 1].quantile + m_elements[ix - 1].norm_weight;
  }
}

WeightedSnapshot::WeightedSnapshot(const WeightedSnapshot&) = default;
WeightedSnapshot::WeightedSnapshot(WeightedSnapshot&&) = default;

WeightedSnapshot::~WeightedSnapshot() = default;

double WeightedSnapshot::get_value(double quantile) const
{
  if (quantile < 0 || quantile > 1.0 || isnan(quantile))
  {
    throw std::domain_error{"Quantile must be between 0.0 and 1.0"};
  }

  if (size() == 0)
  {
    return 0.0;
  }

  auto lb = std::lower_bound(
    std::begin(m_elements), std::end(m_elements), quantile,
    [](const Element& element, const double& q) { return element.quantile < q; }
  );

  if (lb == std::begin(m_elements))
  {
    // All values are of a greater quantile than requested; just return
    // the least.
    return get_min();
  }

  if (lb == std::end(m_elements))
  {
    // All values have a smaller quantile than requested; return the max.
    return get_max();
  }

  // At this point lb is pointed to the first element *not less than* the request
  // quantile.  Back up one and return its value.
  if (!HasEquivalentOrder(quantile, lb->quantile))
  {
    --lb;
  }

  return lb->value;
}

size_t WeightedSnapshot::size() const
{
  return m_elements.size();
}

long WeightedSnapshot::get_min() const
{
  if (size() == 0)
  {
    return 0;
  }

  return m_elements.front().value;
}

double WeightedSnapshot::get_mean() const
{
  double sum = 0.0;
  for (auto&& elem : m_elements)
  {
    sum += elem.value * elem.norm_weight;
  }
  return sum;
}

long WeightedSnapshot::get_max() const
{
  if (size() == 0)
  {
    return 0;
  }

  return m_elements.back().value;
}

double WeightedSnapshot::get_std_dev() const
{
  if (m_elements.size() <= 1)
  {
    return 0.0;
  }

  const double mean = get_mean();
  double variance = 0.0;

  for (auto&& elem : m_elements)
  {
    double diff = elem.value - mean;
    variance += elem.norm_weight * diff * diff;
  }

  return sqrt(variance);
}

const std::vector<long> WeightedSnapshot::get_values() const
{
  std::vector<long> values;
  values.reserve(m_elements.size());
  std::transform(
      std::begin(m_elements),
      std::end(m_elements),
      std::back_inserter(values),
      [](const Element& elem) { return elem.value; }
  );
  return values;
}

}
