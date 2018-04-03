#  Copyright 2018 Benjamin Bader
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

set(METRICS_SOURCES
    Clock.cc
    Counter.cc
    ExponentiallyDecayingReservoir.cc
    EWMA.cc
    Gauge.cc
    Histogram.cc
    Meter.cc
    Registry.cc
    Timer.cc
    WeightedSnapshot.cc
)

set(METRICS_HEADERS
    Clock.h
    Counter.h
    ExponentiallyDecayingReservoir.h
    EWMA.h
    Gauge.h
    Histogram.h
    Meter.h
    Registry.h
    Reporter.h
    Reservoir.h
    Snapshot.h
    Timer.h
    WeightedSnapshot.h
)

set(METRICS_TEST_SOURCES
    ManualClock.cc
)

set(METRICS_TEST_HEADERS
    ManualClock.h
)

add_library(metrics STATIC ${METRICS_SOURCES})
target_include_directories(metrics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

cppmetrics_test(
  TARGET counter
  SOURCES CounterTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics
)

cppmetrics_test(
  TARGET edr
  SOURCES ExponentiallyDecayingReservoirTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics
)

cppmetrics_test(
  TARGET ewma
  SOURCES EWMATests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics
)

cppmetrics_test(
  TARGET histogram
  SOURCES HistogramTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics
)

cppmetrics_test(
  TARGET manual_clock
  SOURCES ManualClockTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics
)

cppmetrics_test(
  TARGET meter
  SOURCES MeterTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics
)

cppmetrics_test(
  TARGET registry
  SOURCES RegistryTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics
)

cppmetrics_test(
  TARGET snapshot
  SOURCES WeightedSnapshotTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics
)

cppmetrics_test(
  TARGET timer
  SOURCES TimerTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics
)
