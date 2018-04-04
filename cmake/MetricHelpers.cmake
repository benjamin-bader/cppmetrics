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

        add_executable(${_NAME}_test_exe ${CPPMETRICS_TEST_SOURCES})
        target_link_libraries(${_NAME}_test_exe PUBLIC ${CPPMETRICS_TEST_PUBLIC_LIBRARIES} gtest_main gtest gmock)
        target_include_directories(${_NAME}_test_exe
            PUBLIC ${CPPMETRICS_TEST_PUBLIC_INCLUDE_DIRS}
            PRIVATE ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS}
        )

        add_test(${_NAME}_test ${_NAME}_test_exe --gtest_output=xml:${_NAME}_test_result.xml)
    endif(BUILD_TESTS)
endfunction(cppmetrics_test)
