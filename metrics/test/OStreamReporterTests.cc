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

#include "gtest/gtest.h"

#include <memory>
#include <sstream>
#include <string>

#include <metrics/metrics.h>

namespace cppmetrics {

class OStreamReporterTests : public testing::Test
{
protected:
  virtual void SetUp()
  {
    registry = std::make_shared<Registry>();
    ss.clear();
    reporter = new OStreamReporter{ss, registry};
  }

  virtual void TearDown()
  {
    delete reporter;
  }

  std::shared_ptr<Registry> registry;
  std::stringstream ss;
  OStreamReporter* reporter;
};

TEST_F(OStreamReporterTests, counter_reporting)
{
  registry->counter("test.ctr.1")->inc(10);
  registry->counter("test.ctr.2")->inc(15);

  reporter->report();

  EXPECT_EQ("test.ctr.1\t10\ntest.ctr.2\t15\n", ss.str());
}

}
