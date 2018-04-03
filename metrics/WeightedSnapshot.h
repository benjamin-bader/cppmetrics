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

#ifndef CPPMETRICS_METRICS_WEIGHTEDSNAPSHOT_H
#define CPPMETRICS_METRICS_WEIGHTEDSNAPSHOT_H

#include <vector>

#include "Snapshot.h"

namespace cppmetrics {

class WeightedSample
{
public:
    WeightedSample(); // for btree only
    WeightedSample(long value, double weight);

    WeightedSample(const WeightedSample&);
    WeightedSample(WeightedSample&&);

    WeightedSample& operator=(const WeightedSample&);
    WeightedSample& operator=(WeightedSample&&);

    long get_value() const noexcept
    {
        return m_value;
    }

    double get_weight() const noexcept
    {
        return m_weight;
    }

    void set_weight(double weight) noexcept
    {
      m_weight = weight;
    }

private:
    long m_value;
    double m_weight;
};

class WeightedSnapshot : public Snapshot
{
public:
  template <typename InputIterator>
  WeightedSnapshot(InputIterator begin, InputIterator end);
  WeightedSnapshot(std::vector<WeightedSample>&&);

  WeightedSnapshot(const WeightedSnapshot&);
  WeightedSnapshot(WeightedSnapshot&&);

  ~WeightedSnapshot();

public:
  double get_value(double quantile) const override;

  size_t size() const override;
  long get_min() const override;
  double get_mean() const override;
  long get_max() const override;
  double get_std_dev() const override;
  const std::vector<long> get_values() const override;

private:
  struct Element
  {
    long value;
    double norm_weight;
    double quantile;
  };

  std::vector<Element> m_elements;
};

template <typename InputIterator>
WeightedSnapshot::WeightedSnapshot(InputIterator begin, InputIterator end)
    : WeightedSnapshot(std::vector<WeightedSample>{begin, end})
{}

}

#endif
