#pragma once

#include <string> 
#include <chrono> 

namespace VitalMonitoring { 

    namespace VitalRanges {
        // Temperature in Fahrenheit
        const float TEMP_CRITICAL_HIGH = 102.0f;
        const float TEMP_CRITICAL_LOW = 95.0f;

        // Pulse Rate in beats per minute
        const float PULSE_RATE_CRITICAL_HIGH = 100.0f;
        const float PULSE_RATE_CRITICAL_LOW = 60.0f;

        // Oxygen Saturation percentage
        const float SPO2_CRITICAL_LOW = 90.0f;
    }

    namespace AlertSettings {
        const int FLASH_COUNT = 6;
        const std::chrono::seconds FLASH_DELAY = std::chrono::seconds(1);
    }

    // --- VitalType Enum ---
    enum class VitalType {
        Temperature,
        PulseRate,
        SPO2
    };

    // --- IVitalReporter Interface ---
    class IVitalReporter {
    public:
        virtual ~IVitalReporter() = default;
        virtual void reportCritical(VitalType type, float value, const std::string& message) = 0;
    };

    // --- Default Console Reporter Declaration ---
    class ConsoleVitalReporter : public IVitalReporter {
    public:
        void reportCritical(VitalType type, float value, const std::string& message) override;
    private:
        void displayCriticalAlertAnimation(); // Helper for console animation
    };

    IVitalReporter* getDefaultConsoleReporter();

} 
int vitalsOk(float temperature, float pulseRate, float spo2, VitalMonitoring::IVitalReporter* reporter = nullptr);
