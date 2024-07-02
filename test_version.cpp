
#include "lib.h"
#include "gtest/gtest.h"


// Demonstrate some basic assertions.
TEST(VersionTest, BasicAssertions) {
    // Expect equality
    ASSERT_TRUE(version() > 0);
}