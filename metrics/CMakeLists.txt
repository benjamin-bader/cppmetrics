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

project(metrics VERSION 0.1 LANGUAGES CXX)

set(METRICS_SOURCES
    src/Clock.cc
    src/Counter.cc
    src/ExponentiallyDecayingReservoir.cc
    src/EWMA.cc
    src/Gauge.cc
    src/Histogram.cc
    src/Meter.cc
    src/Registry.cc
    src/Timer.cc
    src/WeightedSnapshot.cc
)
set(METRICS_TEST_SOURCES
    test/ManualClock.cc
)

add_library(metrics SHARED ${METRICS_SOURCES})
add_library(metrics_static STATIC ${METRICS_SOURCES})

target_include_directories(metrics
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>

  PRIVATE
  src
)

target_include_directories(metrics_static
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>

  PRIVATE
  src
)

set(METRICS_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
set(METRICS_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
set(METRICS_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include)

install(TARGETS metrics metrics_static EXPORT MetricsLibraryConfig
  ARCHIVE DESTINATION ${METRICS_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${METRICS_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${METRICS_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${METRICS_INSTALL_INCLUDEDIR})

install(EXPORT MetricsLibraryConfig DESTINATION share/MetricsLibrary/cmake)

export(TARGETS metrics metrics_static FILE MetricsLibraryConfig.cmake)

cppmetrics_test(
  TARGET counter
  SOURCES test/CounterTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics_static
)

cppmetrics_test(
  TARGET edr
  SOURCES test/ExponentiallyDecayingReservoirTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics_static
)

cppmetrics_test(
  TARGET ewma
  SOURCES test/EWMATests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics_static
)

cppmetrics_test(
  TARGET histogram
  SOURCES test/HistogramTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics_static
)

cppmetrics_test(
  TARGET manual_clock
  SOURCES test/ManualClockTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics_static
)

cppmetrics_test(
  TARGET meter
  SOURCES test/MeterTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics_static
)

cppmetrics_test(
  TARGET registry
  SOURCES test/RegistryTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics_static
)

cppmetrics_test(
  TARGET snapshot
  SOURCES test/WeightedSnapshotTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics_static
)

cppmetrics_test(
  TARGET timer
  SOURCES test/TimerTests.cc ${METRICS_TEST_SOURCES}
  PUBLIC_LIBRARIES metrics_static
)
