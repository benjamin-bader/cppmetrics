#ifndef CPPMETRICS_METRICS_RESERVOIR_H
#define CPPMETRICS_METRICS_RESERVOIR_H

#include <memory>

namespace cppmetrics {

class Snapshot;

class Reservoir
{
public:
    virtual ~Reservoir() = default;

    virtual size_t size() const = 0;
    virtual void update(long value) = 0;
    virtual std::shared_ptr<Snapshot> get_snapshot() = 0;
};

}

#endif // CPPMETRICS_METRICS_RESERVOIR_H
