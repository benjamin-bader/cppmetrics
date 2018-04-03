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

#include "Counter.h"

#include "gtest/gtest.h"

namespace cppmetrics {

TEST(CounterTests, increments)
{
  Counter ctr;
  EXPECT_EQ(0, ctr.get_count());

  ctr.inc();
  EXPECT_EQ(1, ctr.get_count());

  ctr.inc(10);
  EXPECT_EQ(11, ctr.get_count());
}

TEST(CounterTests, decrements)
{
  Counter ctr;
  EXPECT_EQ(0, ctr.get_count());

  ctr.dec();
  EXPECT_EQ(-1, ctr.get_count());

  ctr.dec(10);
  EXPECT_EQ(-11, ctr.get_count());
}

}
