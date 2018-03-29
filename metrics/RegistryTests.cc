#include "Registry.h"

#include "gtest/gtest.h"

namespace cppmetrics {

TEST(RegistryTest, foo)
{
  Registry registry;

  registry.histogram("foo");

  EXPECT_EQ(1, registry.get_histograms().size());
}

}
