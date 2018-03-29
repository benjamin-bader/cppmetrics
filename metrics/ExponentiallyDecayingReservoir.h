#ifndef CPPMETRICS_METRICS_EXPONENTIALLYDECAYINGRESERVOIR_H
#define CPPMETRICS_METRICS_EXPONENTIALLYDECAYINGRESERVOIR_H

#include <atomic>
#include <ctime>
#include <memory>
#include <mutex>

#include "btree/btree_map.h"

#include "Reservoir.h"
#include "WeightedSnapshot.h"

namespace cppmetrics {

class ExponentiallyDecayingReservoir : public Reservoir
{
  static const size_t kDefaultSize;
  static const double kDefaultAlpha;

public:
    ExponentiallyDecayingReservoir(size_t size = kDefaultSize, double alpha = kDefaultAlpha);
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
    std::time_t m_start;
    std::time_t m_next_rescale_time;
    size_t m_size;
    double m_alpha;
    btree::btree_map<double, WeightedSample> m_samples;
};

}

#endif // CPPMETRICS_EXPONENTIALLYDECAYINGRESERVOIR_H
