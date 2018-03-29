#ifndef CPPMETRICS_METRICS_CLOCK_H
#define CPPMETRICS_METRICS_CLOCK_H

#include <chrono>

namespace cppmetrics {

class Clock
{
public:
  virtual std::chrono::nanoseconds tick();
};

Clock* GetDefaultClock();

}

#endif
