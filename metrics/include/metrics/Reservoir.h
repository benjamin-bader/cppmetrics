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

#ifndef CPPMETRICS_METRICS_RESERVOIR_H
#define CPPMETRICS_METRICS_RESERVOIR_H

#include <cstddef>
#include <memory>

namespace cppmetrics {

class Snapshot;

class Reservoir
{
public:
    virtual ~Reservoir() = default;

    virtual std::size_t size() const = 0;
    virtual void update(long value) = 0;
    virtual std::shared_ptr<Snapshot> get_snapshot() = 0;
};

}

#endif // CPPMETRICS_METRICS_RESERVOIR_H
