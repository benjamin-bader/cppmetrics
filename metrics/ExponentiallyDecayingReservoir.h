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

#ifndef CPPMETRICS_METRICS_EXPONENTIALLYDECAYINGRESERVOIR_H
#define CPPMETRICS_METRICS_EXPONENTIALLYDECAYINGRESERVOIR_H

#include <atomic>
#include <ctime>
#include <memory>
#include <mutex>
#include <map>

#include "Reservoir.h"
#include "WeightedSnapshot.h"

namespace cppmetrics {

class Clock;

class ExponentiallyDecayingReservoir : public Reservoir
{
  static const size_t kDefaultSize;
  static const double kDefaultAlpha;

public:
    ExponentiallyDecayingReservoir(size_t size = kDefaultSize, double alpha = kDefaultAlpha, Clock* clock = nullptr);
    ExponentiallyDecayingReservoir(ExponentiallyDecayingReservoir&&);

    ExponentiallyDecayingReservoir& operator=(ExponentiallyDecayingReservoir&&);

public:
    size_t size() const override;
    void update(long value) override;

    std::shared_ptr<Snapshot> get_snapshot() override;

private:
    void rescale_if_needed();
    void rescale(std::time_t now, std::time_t next);

private:
    std::mutex m_mutex;
    std::atomic_long m_count;
    Clock* m_clock;
    std::time_t m_start;
    std::time_t m_next_rescale_time;
    size_t m_size;
    double m_alpha;
    std::map<double, WeightedSample> m_samples;
};

}

#endif // CPPMETRICS_EXPONENTIALLYDECAYINGRESERVOIR_H
