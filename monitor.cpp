#include "monitor.h" 
#include <iostream>  
#include <thread>    

namespace VitalMonitoring {

    // --- ConsoleVitalReporter Implementation ---
    void ConsoleVitalReporter::displayCriticalAlertAnimation() {
        for (int i = 0; i < VitalMonitoring::AlertSettings::FLASH_COUNT; ++i) {
            std::cout << "\r* " << std::flush;
            std::this_thread::sleep_for(VitalMonitoring::AlertSettings::FLASH_DELAY);
            std::cout << "\r *" << std::flush;
            std::this_thread::sleep_for(VitalMonitoring::AlertSettings::FLASH_DELAY);
        }
    }

    void ConsoleVitalReporter::reportCritical(VitalType type, float value, const std::string& message) {
        std::cout << message << " (Value: " << value << ")\n";
        displayCriticalAlertAnimation();
    }

    IVitalReporter* getDefaultConsoleReporter() {
        static ConsoleVitalReporter reporter; // Static to ensure single instance and lifetime
        return &reporter;
    }

    bool checkTemperature(float temperature, IVitalReporter* reporter) {
        if (temperature > VitalRanges::TEMP_CRITICAL_HIGH || temperature < VitalRanges::TEMP_CRITICAL_LOW) {
            reporter->reportCritical(VitalType::Temperature, temperature, "Temperature is critical!");
            return false;
        }
        return true;
    }

    bool checkPulseRate(float pulseRate, IVitalReporter* reporter) {
        if (pulseRate < VitalRanges::PULSE_RATE_CRITICAL_LOW || pulseRate > VitalRanges::PULSE_RATE_CRITICAL_HIGH) {
            reporter->reportCritical(VitalType::PulseRate, pulseRate, "Pulse Rate is out of range!");
            return false;
        }
        return true;
    }

    bool checkSpo2(float spo2, IVitalReporter* reporter) {
        if (spo2 < VitalRanges::SPO2_CRITICAL_LOW) {
            reporter->reportCritical(VitalType::SPO2, spo2, "Oxygen Saturation out of range!");
            return false;
        }
        return true;
    }

} 
// --- Main vitalsOk Function (Global Namespace) ---
int vitalsOk(float temperature, float pulseRate, float spo2, VitalMonitoring::IVitalReporter* reporter) {
    if (!reporter) {
        reporter = VitalMonitoring::getDefaultConsoleReporter();
    }

    // Check each vital using the helper functions and provided/default reporter
    if (!VitalMonitoring::checkTemperature(temperature, reporter)) {
        return 0;
    }
    if (!VitalMonitoring::checkPulseRate(pulseRate, reporter)) {
        return 0;
    }
    if (!VitalMonitoring::checkSpo2(spo2, reporter)) {
        return 0;
    }

    return 1; // All vitals are within the healthy range
}
