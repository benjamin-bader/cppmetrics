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

#include "ExponentiallyDecayingReservoir.h"

#include "gtest/gtest.h"

namespace cppmetrics {

TEST(EDRTest, sample_100_of_1000)
{
  ExponentiallyDecayingReservoir reservoir(100, 0.99);

  for (int i = 0; i < 1000; ++i)
  {
    reservoir.update(i);
  }

  EXPECT_EQ(100, reservoir.size());

  auto snapshot = reservoir.get_snapshot();
  EXPECT_EQ(100, snapshot->size());

  for (long value : snapshot->get_values())
  {
    EXPECT_LE(0, value);
    EXPECT_GT(1000, value);
  }
}

}
