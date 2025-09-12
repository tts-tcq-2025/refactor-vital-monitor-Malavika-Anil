#pragma once
#ifndef MONITOR_H
#define MONITOR_H

// Declare individual vital checks
int tempOk(float temperature);
int pulseRateOk(int pulse);
int spo2Ok(int spo2);

// Declare combined vital check
int vitalsOk(float temperature, int pulseRate, int spo2);

#endif




