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

#ifndef CPPMETRICS_METRICS_SNAPSHOT_H
#define CPPMETRICS_METRICS_SNAPSHOT_H

#include <cstddef>
#include <vector>

namespace cppmetrics {

class Snapshot
{
public:
  virtual ~Snapshot() {}

  virtual double get_value(double quantile) const = 0;

  virtual std::size_t size() const = 0;
  virtual long get_min() const = 0;
  virtual double get_mean() const = 0;
  virtual long get_max() const = 0;
  virtual double get_std_dev() const = 0;
  virtual const std::vector<long> get_values() const = 0;
  
  virtual double get_median() const
  {
    return get_value(0.5);
  }

  virtual double get_p75() const
  {
    return get_value(0.75);
  }

  virtual double get_p95() const
  {
    return get_value(0.95);
  }

  virtual double get_p98() const
  {
    return get_value(0.98);
  }

  virtual double get_p99() const
  {
    return get_value(0.99);
  }

  virtual double get_p999() const
  {
    return get_value(0.999);
  }
};

}

#endif
