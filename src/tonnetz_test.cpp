#include <gtest/gtest.h>
#include "tonnetz.h"

TEST(TonnetzTest, BasicAssertions) {
  Tonnetz testTonnetz = Tonnetz();

  // horizontal
  testTonnetz.pos = uvec2(0, 0);
  EXPECT_NEAR(testTonnetz.getFreq(testTonnetz.pos), 130.8, 0.2) << "Origin incorrect";
  testTonnetz.pos = uvec2(1, 0);
  EXPECT_NEAR(testTonnetz.getFreq(testTonnetz.pos), 196, 0.2);
  testTonnetz.pos = uvec2(2, 0);
  EXPECT_NEAR(testTonnetz.getFreq(testTonnetz.pos), 146.8, 0.2);

  // vertical
  testTonnetz.pos = uvec2(0, 1);
  EXPECT_NEAR(testTonnetz.getFreq(testTonnetz.pos), 155.5, 0.2);
  testTonnetz.pos = uvec2(0, 2);
  EXPECT_NEAR(testTonnetz.getFreq(testTonnetz.pos), 185, 0.2);
  testTonnetz.pos = uvec2(0, 3);
  EXPECT_NEAR(testTonnetz.getFreq(testTonnetz.pos), 130.8, 0.2);

  // superposition
  testTonnetz.pos = uvec2(1, 1);
  EXPECT_NEAR(testTonnetz.getFreq(testTonnetz.pos), 233, 0.2);
  testTonnetz.pos = uvec2(1, 2);
  EXPECT_NEAR(testTonnetz.getFreq(testTonnetz.pos), 138.6, 0.2);
  testTonnetz.pos = uvec2(2, 1);
  EXPECT_NEAR(testTonnetz.getFreq(testTonnetz.pos), 174.6, 0.2);
}