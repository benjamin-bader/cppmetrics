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
    Metric.h
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
  TARGET edr
  SOURCES ExponentiallyDecayingReservoirTests.cc
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
