#include 
#include 

#include <vector>
#include <string>

class MockVitalReporter : public VitalMonitoring::IVitalReporter {
public:
    struct ReportEntry {
        VitalMonitoring::VitalType type;
        float value;
        std::string message;
    };

    std::vector<ReportEntry> reportedIssues;

    void reportCritical(VitalMonitoring::VitalType type, float value, const std::string& message) override {
        reportedIssues.push_back({type, value, message});
    }

    void clear() {
        reportedIssues.clear();
    }
};

class MonitorTest : public ::testing::Test {
protected:
    MockVitalReporter mockReporter;
    void SetUp() override {
        mockReporter.clear();
    }
};

TEST_F(MonitorTest, NotOkWhenAnyVitalIsOffRange) {
    // Test case 1: Temperature is out of range (high)
    ASSERT_EQ(0, vitalsOk(103.0f, 70.0f, 98.0f, &mockReporter));
    ASSERT_EQ(1, mockReporter.reportedIssues.size());
    ASSERT_EQ(VitalMonitoring::VitalType::Temperature, mockReporter.reportedIssues[0].type);
    ASSERT_EQ("Temperature is critical!", mockReporter.reportedIssues[0].message);
    mockReporter.clear(); // Clear for next sub-test within this test function

    // Test case 2: Pulse Rate is out of range (high)
    ASSERT_EQ(0, vitalsOk(98.0f, 102.0f, 98.0f, &mockReporter));
    ASSERT_EQ(1, mockReporter.reportedIssues.size());
    ASSERT_EQ(VitalMonitoring::VitalType::PulseRate, mockReporter.reportedIssues[0].type);
    ASSERT_EQ("Pulse Rate is out of range!", mockReporter.reportedIssues[0].message);
    mockReporter.clear();

    // Test case 3: Oxygen Saturation is out of range (low)
    ASSERT_EQ(0, vitalsOk(98.0f, 70.0f, 70.0f, &mockReporter));
    ASSERT_EQ(1, mockReporter.reportedIssues.size());
    ASSERT_EQ(VitalMonitoring::VitalType::SPO2, mockReporter.reportedIssues[0].type);
    ASSERT_EQ("Oxygen Saturation out of range!", mockReporter.reportedIssues[0].message);
    mockReporter.clear();

    // Test case 4: All vitals are within range - no issues should be reported
    ASSERT_EQ(1, vitalsOk(98.1f, 70.0f, 98.0f, &mockReporter));
    ASSERT_TRUE(mockReporter.reportedIssues.empty());
}

TEST_F(MonitorTest, OkWhenAllVitalsAreInRange) {
    ASSERT_EQ(1, vitalsOk(98.6f, 75.0f, 95.0f, &mockReporter));
    ASSERT_TRUE(mockReporter.reportedIssues.empty());

    ASSERT_EQ(1, vitalsOk(95.1f, 60.1f, 90.1f, &mockReporter)); // Boundary checks for low values
    ASSERT_TRUE(mockReporter.reportedIssues.empty());

    ASSERT_EQ(1, vitalsOk(101.9f, 99.9f, 100.0f)); // Boundary checks for high values
    ASSERT_TRUE(mockReporter.reportedIssues.empty());
}

