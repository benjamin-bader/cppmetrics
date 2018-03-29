#ifndef CPPMETRICS_METRIC_H
#define CPPMETRICS_METRIC_H

#include <map>
#include <string>

namespace cppmetrics {

class Metric
{
public:
  Metric(const std::string& name, const std::map<std::string, std::string>& labels = {});
  virtual ~Metric() {}

  const std::string& name() const
  {
    return m_name;
  }

  const std::map<std::string, std::string>& labels() const
  {
    return m_labels;
  }

private:
  std::string m_name;
  std::map<std::string, std::string> m_labels;
};

}

#endif
