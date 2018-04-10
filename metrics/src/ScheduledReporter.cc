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

#include <metrics/ScheduledReporter.h>

#include <utility>

#include <metrics/Registry.h>

namespace cppmetrics {

ScheduledReporter::ScheduledReporter(std::unique_ptr<Reporter>&& reporter, const std::chrono::system_clock::duration& duration)
    : m_reporter(std::move(reporter))
    , m_interval(duration)
    , m_mutex()
    , m_cv()
    , m_is_running(false)
{
}

ScheduledReporter::~ScheduledReporter()
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_is_running = false;
  m_cv.notify_all();
  lock.unlock();

  if (m_reporter_thread.joinable())
  {
    m_reporter_thread.join();
  }
}

void ScheduledReporter::start()
{
  std::unique_lock<std::mutex> lock(m_mutex);
  if (!m_is_running)
  {
    m_is_running = true;
    m_reporter_thread = std::thread([&]() {
      loop_while_running();
    });
  }
}

void ScheduledReporter::loop_while_running()
{
  while (true)
  {
    auto start = std::chrono::system_clock::now();
    auto next_report_time = start + m_interval;

    std::unique_lock<std::mutex> lock(m_mutex);

    while (m_is_running && next_report_time > std::chrono::system_clock::now())
    {
      m_cv.wait_until(lock, next_report_time, [&]() {
        // Predicate returns 'false' if waiting should continue.
        // We want to stop waiting if we have ceased running,
        // and to continue waiting while we are running.
        return !m_is_running;
      });
    }

    if (!m_is_running)
    {
      return;
    }

    lock.unlock();
    report();
  }
}

void ScheduledReporter::report()
{
  m_reporter->report();
}

} // namespace cppmetrics
