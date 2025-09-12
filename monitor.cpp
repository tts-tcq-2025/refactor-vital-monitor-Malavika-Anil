#include "./monitor.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;
constexpr float TEMP_LOW   = 95.0f;
constexpr float TEMP_HIGH  = 102.0f;
constexpr float PULSE_LOW  = 60.0f;
constexpr float PULSE_HIGH = 100.0f;
constexpr float SPO2_MIN   = 90.0f;
constexpr int OK     = 1;
constexpr int NOT_OK = 0;

// Common blinking alert
void blinkAlert() {
    for (int i = 0; i < 6; i++) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
    cout << "\r  \r" << flush;
}

// Generic alert function
void alertOutOfRange(const std::string& vitalName) {
    cout << vitalName << " is out of range!\n";
    blinkAlert();
}

// Generic range checker
int checkRange(float value, float min, float max, const std::string& vitalName) {
    if (value < min || value > max) {
        alertOutOfRange(vitalName);
        return NOT_OK;
    }
    return OK;
}

// Vitals-specific checks
int tempOk(float temperature) {
    return checkRange(temperature, TEMP_LOW, TEMP_HIGH, "Temperature");
}

int pulseRateOk(float pulseRate) {
    return checkRange(pulseRate, PULSE_LOW, PULSE_HIGH, "Pulse Rate");
}

int spo2Ok(float spo2) {
    // only lower bound matters for SpO2
    return checkRange(spo2, SPO2_MIN, 9999.0f, "Oxygen Saturation");
}

// Final combined check
int vitalsOk(float temperature, float pulseRate, float spo2) {
    return tempOk(temperature) && pulseRateOk(pulseRate) && spo2Ok(spo2);
}
