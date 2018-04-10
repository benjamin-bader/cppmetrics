//  Copyright 2018 Benjamin Bader
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include <metrics/OStreamReporter.h>

#include <iostream>
#include <utility>

#include <metrics/metrics.h>

namespace cppmetrics {

OStreamReporter::OStreamReporter(std::ostream& output, const std::shared_ptr<Registry>& registry)
    : m_output(output)
    , m_registry(registry)
{
}

OStreamReporter::OStreamReporter(OStreamReporter&&) = default;

OStreamReporter::~OStreamReporter() = default;

void OStreamReporter::report()
{
  for (auto&& counter : m_registry->get_counters())
  {
    m_output << counter.first << "\t" << counter.second->get_count() << "\n";
  }

  for (auto&& gauge : m_registry->get_gauges())
  {
    //m_output << counter.first << "\t" << counter.second->
  }

  for (auto&& meter : m_registry->get_meters())
  {
    m_output << meter.first << ".count\t" << meter.second->get_count() << "\n";
    m_output << meter.first << ".mean\t" << meter.second->get_mean_rate() << "\n";
    m_output << meter.first << ".m1\t" << meter.second->get_m1_rate() << "\n";
    m_output << meter.first << ".m5\t" << meter.second->get_m5_rate() << "\n";
    m_output << meter.first << ".m15\t" << meter.second->get_m15_rate() << "\n";
  }

  for (auto&& histogram : m_registry->get_histograms())
  {
    auto snapshot = histogram.second->get_snapshot();
    m_output << histogram.first << ".count\t" << histogram.second->get_count() << "\n";
    m_output << histogram.first << ".p75\t" << snapshot->get_p75() << "\n";
    m_output << histogram.first << ".p95\t" << snapshot->get_p95() << "\n";
    m_output << histogram.first << ".p99\t" << snapshot->get_p99() << "\n";
  }

  for (auto&& timer : m_registry->get_timers())
  {
    auto snapshot = timer.second->get_snapshot();
    m_output << timer.first << ".count\t" << timer.second->get_count() << "\n";
    m_output << timer.first << ".m1\t" << timer.second->get_m1_rate() << "\n";
    m_output << timer.first << ".m5\t" << timer.second->get_m5_rate() << "\n";
    m_output << timer.first << ".m15\t" << timer.second->get_m15_rate() << "\n";
    m_output << timer.first << ".p75\t" << snapshot->get_p75() << "\n";
    m_output << timer.first << ".p95\t" << snapshot->get_p95() << "\n";
    m_output << timer.first << ".p99\t" << snapshot->get_p99() << "\n";
  }
}

}
