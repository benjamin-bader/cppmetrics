#include "Histogram.h"

#include <utility>

namespace cppmetrics {

Histogram::Histogram(std::unique_ptr<Reservoir>&& reservoir)
    : m_counter(0)
    , m_reservoir(std::move(reservoir))
{}

void Histogram::update(long n)
{
  m_counter += n;
  m_reservoir->update(n);
}

long Histogram::get_count() const
{
  return m_counter.load();
}

std::shared_ptr<Snapshot> Histogram::get_snapshot()
{
  return m_reservoir->get_snapshot();
}

}
