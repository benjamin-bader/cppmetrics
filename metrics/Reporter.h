#ifndef CPPMETRICS_REPORTER_H
#define CPPMETRICS_REPORTER_H

namespace cppmetrics {

class Metric;

class Reporter
{
public:
  virtual ~Reporter() {}

  virtual void report(Metric& metric) = 0;
};

}

#endif
