#include <gtest/gtest.h>
#include "./monitor.h"

TEST(MonitorTest,VitalsChecks){
  EXPECT_FALSE(vitalsOk(99,102,70));   // pulse high + spo2 low
  EXPECT_FALSE(vitalsOk(103,80,95));   // temp high
  EXPECT_FALSE(vitalsOk(94,80,95));    // temp low
  EXPECT_FALSE(vitalsOk(98.6,55,95));  // pulse low
  EXPECT_FALSE(vitalsOk(98.6,75,85));  // spo2 low
  EXPECT_TRUE(vitalsOk(98.1,70,98));   // all normal
}


