#include "monitor.h" // Include its own header, which now contains constants and interfaces
#include <iostream>  // For std::cout, std::flush
#include <thread>    // For std::this_thread::sleep_for
#include <vector>    // For std::vector
#include <functional> // For std::function

namespace VitalMonitoring { // All implementations reside in this namespace

    // --- ConsoleVitalReporter Implementation ---
    void ConsoleVitalReporter::displayCriticalAlertAnimation() {
        // CC = 2
        for (int i = 0; i < VitalMonitoring::AlertSettings::FLASH_COUNT; ++i) {
            std::cout << "\r* " << std::flush;
            std::this_thread::sleep_for(VitalMonitoring::AlertSettings::FLASH_DELAY);
            std::cout << "\r *" << std::flush;
            std::this_thread::sleep_for(VitalMonitoring::AlertSettings::FLASH_DELAY);
        }
    }

    void ConsoleVitalReporter::reportCritical(VitalType type, float value, const std::string& message) {
        // CC = 1
        std::cout << message << " (Value: " << value << ")\n";
        displayCriticalAlertAnimation();
    }

    IVitalReporter* getDefaultConsoleReporter() {
        // CC = 1
        static ConsoleVitalReporter reporter;
        return &reporter;
    }

    // --- Helper Functions to Determine Criticality (CC = 2) ---
    bool isTemperatureCritical(float temperature) {
        // CC = 2
        if (temperature > VitalRanges::TEMP_CRITICAL_HIGH || temperature < VitalRanges::TEMP_CRITICAL_LOW) {
            return true;
        }
        return false;
    }

    bool isPulseRateCritical(float pulseRate) {
        // CC = 2
        if (pulseRate < VitalRanges::PULSE_RATE_CRITICAL_LOW || pulseRate > VitalRanges::PULSE_RATE_CRITICAL_HIGH) {
            return true;
        }
        return false;
    }

    bool isSpo2Critical(float spo2) {
        // CC = 2
        if (spo2 < VitalRanges::SPO2_CRITICAL_LOW) {
            return true;
        }
        return false;
    }

    // --- Critical Check and Report Function (CC = 2) ---
    bool performVitalCheckAndReport(VitalType type, float value, std::function<bool(float)> isCriticalFn,
                                   const std::string& criticalMessage, IVitalReporter* reporter) {
        // CC = 2
        if (isCriticalFn(value)) {
            reporter->reportCritical(type, value, criticalMessage);
            return false; // Vital is critical
        }
        return true; // Vital is OK
    }

    // --- NEW HELPER: Process all vital checks ---
    // Encapsulates the iteration over vital checks
    bool processAllVitalChecks(float temperature, float pulseRate, float spo2, IVitalReporter* reporter) {
        // CC = 2 (for loop + if inside loop)
        bool allVitalsOk = true;

        std::vector<std::function<bool()>> checks;

        checks.push_back([&]() {
            return performVitalCheckAndReport(
                VitalType::Temperature, temperature,
                isTemperatureCritical, "Temperature is critical!", reporter);
        });
        checks.push_back([&]() {
            return performVitalCheckAndReport(
                VitalType::PulseRate, pulseRate,
                isPulseRateCritical, "Pulse Rate is out of range!", reporter);
        });
        checks.push_back([&]() {
            return performVitalCheckAndReport(
                VitalType::SPO2, spo2,
                isSpo2Critical, "Oxygen Saturation out of range!", reporter);
        });

        for (const auto& check : checks) {
            if (!check()) { // Branch here
                allVitalsOk = false;
            }
        }
        return allVitalsOk;
    }


} // namespace VitalMonitoring

// --- Main vitalsOk Function (Further Refactored for Minimized Cyclomatic Complexity) ---
int vitalsOk(float temperature, float pulseRate, float spo2, VitalMonitoring::IVitalReporter* reporter) {
    // CC = 2
    if (!reporter) { // Branch 1
        reporter = VitalMonitoring::getDefaultConsoleReporter();
    }

    bool allVitalsOk = VitalMonitoring::processAllVitalChecks(temperature, pulseRate, spo2, reporter);

    return allVitalsOk ? 1 : 0; // Branch 2 (ternary operator)
}
