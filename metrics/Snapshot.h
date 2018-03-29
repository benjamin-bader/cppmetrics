#ifndef CPPMETRICS_METRICS_SNAPSHOT_H
#define CPPMETRICS_METRICS_SNAPSHOT_H

namespace cppmetrics {

class Snapshot
{
public:
  virtual ~Snapshot() {}

  virtual double get_value(double quantile) const = 0;

  virtual size_t size() const = 0;
  virtual long get_min() const = 0;
  virtual double get_mean() const = 0;
  virtual long get_max() const = 0;
  virtual double get_std_dev() const = 0;
  virtual const std::vector<long> get_values() const = 0;
  
  virtual double get_median() const
  {
    return get_value(0.5);
  }

  virtual double get_p75() const
  {
    return get_value(0.75);
  }

  virtual double get_p95() const
  {
    return get_value(0.95);
  }

  virtual double get_p98() const
  {
    return get_value(0.98);
  }

  virtual double get_p99() const
  {
    return get_value(0.99);
  }

  virtual double get_p999() const
  {
    return get_value(0.999);
  }
};

}

#endif
