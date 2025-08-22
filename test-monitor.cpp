#include <gtest/gtest.h>
#include "./monitor.h"

TEST(MonitorTest,TemperatureChecks){
EXPECT_FALSE(vitalsOk(103.0f,75.0f,95.0f));
EXPECT_FALSE(vitalsOk(94.0f,75.0f,95.0f));
EXPECT_TRUE(vitalsOk(98.6f,75.0f,95.0f));
}

TEST(MonitorTest,PulseChecks){
EXPECT_FALSE(vitalsOk(98.6f,110.0f,95.0f));
EXPECT_FALSE(vitalsOk(98.6f,55.0f,95.0f));
EXPECT_TRUE(vitalsOk(98.6f,75.0f,95.0f));
}

TEST(MonitorTest,Spo2Checks){
EXPECT_FALSE(vitalsOk(98.6f,75.0f,85.0f));
EXPECT_TRUE(vitalsOk(98.6f,75.0f,95.0f));
}

TEST(MonitorTest,AllVitalsNormalOk){
EXPECT_TRUE(vitalsOk(98.1f,70.0f,98.0f));
}

