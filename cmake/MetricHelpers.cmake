include(CMakeParseArguments)

function(cppmetrics_test)
    cmake_parse_arguments(CPPMETRICS_TEST
        ""
        "TARGET"
        "SOURCES;PUBLIC_LIBRARIES;PUBLIC_INCLUDE_DIRS"
        ${ARGN}
    )

    if(BUILD_TESTS)
        set(_NAME ${CPPMETRICS_TEST_TARGET})
        string(TOUPPER ${_NAME} _UPPER_NAME)

        add_executable(${_NAME}_exe ${CPPMETRICS_TEST_SOURCES})
        target_link_libraries(${_NAME}_exe PUBLIC ${CPPMETRICS_TEST_PUBLIC_LIBRARIES} gtest_main gtest gmock)
        target_include_directories(${_NAME}_exe
            PUBLIC ${CPPMETRICS_TEST_PUBLIC_INCLUDE_DIRS}
            PRIVATE ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS}
        )

        add_test(${_NAME}_test ${_NAME}_exe)
    endif(BUILD_TESTS)
endfunction(cppmetrics_test)
