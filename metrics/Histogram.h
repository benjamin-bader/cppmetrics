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

#ifndef CPPMETRICS_METRICS_HISTOGRAM_H
#define CPPMETRICS_METRICS_HISTOGRAM_H

#include <atomic>
#include <memory>

#include "Reservoir.h"
#include "Snapshot.h"

namespace cppmetrics {

class Histogram
{
public:
  Histogram(std::unique_ptr<Reservoir>&& reservoir);

  void update(long n);

  long get_count() const;
  std::shared_ptr<Snapshot> get_snapshot();

private:
  std::atomic_long m_counter;
  std::unique_ptr<Reservoir> m_reservoir;
};

}

#endif
