#ifndef CPPMETRICS_METRICS_MANUALCLOCK_H
#define CPPMETRICS_METRICS_MANUALCLOCK_H

#include "Clock.h"

namespace cppmetrics {

class ManualClock : public Clock
{
public:
  ManualClock(const std::chrono::nanoseconds& now = std::chrono::nanoseconds(0));

  std::chrono::nanoseconds tick() override;

  void add_nanos(long long nanos);
  void add_seconds(int seconds);
  void add_minutes(int minutes);
  void add_hours(int hours);

private:
  std::chrono::nanoseconds m_now;
};

}

#endif
