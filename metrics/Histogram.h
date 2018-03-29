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
