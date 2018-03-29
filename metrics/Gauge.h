#ifndef CPPMETRICS_GAUGE_H
#define CPPMETRICS_GAUGE_H

#include <atomic>
#include <map>
#include <string>

#include "Metric.h"

namespace cppmetrics {

class Gauge
{
public:
  Gauge();

  void set(long value);
  long get();

private:
  std::atomic_long m_value;
};

}

#endif
