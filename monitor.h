#pragma once
#ifndef MONITOR_H
#define MONITOR_H

// Declare individual vital checks
bool tempOk(float temperature);
bool pulseRateOk(int pulse);
bool spo2Ok(int spo2);

// Declare combined vital check
bool vitalsOk(float temperature, int pulseRate, int spo2);

#endif // MONITOR_H
