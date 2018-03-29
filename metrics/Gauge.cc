#include "Gauge.h"

namespace cppmetrics {

Gauge::Gauge()
{

}

void Gauge::set(long value)
{
  m_value.store(value);
}

long Gauge::get()
{
  return m_value.load();
}

}
