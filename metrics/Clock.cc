#include "Clock.h"

namespace cppmetrics {

namespace {

Clock* gDefaultClock = new Clock;

}

std::chrono::nanoseconds Clock::tick()
{
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch());
}

Clock* GetDefaultClock()
{
  return gDefaultClock;
}

}
