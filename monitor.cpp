#include "monitor.h"
#include <iostream>
#include <thread>
#include <vector>
#include <functional>

namespace VitalMonitoring {

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
        static ConsoleVitalReporter reporter;
        return &reporter;
    }

    bool isTemperatureCritical(float temperature) {
        if (temperature > VitalRanges::TEMP_CRITICAL_HIGH || temperature < VitalRanges::TEMP_CRITICAL_LOW) {
            return true;
        }
        return false;
    }

    bool isPulseRateCritical(float pulseRate) {
        if (pulseRate < VitalRanges::PULSE_RATE_CRITICAL_LOW || pulseRate > VitalRanges::PULSE_RATE_CRITICAL_HIGH) {
            return true;
        }
        return false;
    }

    bool isSpo2Critical(float spo2) {
        if (spo2 < VitalRanges::SPO2_CRITICAL_LOW) {
            return true;
        }
        return false;
    }

    bool performVitalCheckAndReport(VitalType type, float value, std::function<bool(float)> isCriticalFn,
                                   const std::string& criticalMessage, IVitalReporter* reporter) {
        if (isCriticalFn(value)) {
            reporter->reportCritical(type, value, criticalMessage);
            return false;
        }
        return true;
    }

    bool processAllVitalChecks(float temperature, float pulseRate, float spo2, IVitalReporter* reporter) {
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
            if (!check()) {
                allVitalsOk = false;
            }
        }
        return allVitalsOk;
    }

} 

int vitalsOk(float temperature, float pulseRate, float spo2, VitalMonitoring::IVitalReporter* reporter) {
    if (!reporter) {
        reporter = VitalMonitoring::getDefaultConsoleReporter();
    }
    bool allVitalsOk = VitalMonitoring::processAllVitalChecks(temperature, pulseRate, spo2, reporter);
    return allVitalsOk ? 1 : 0;
}
