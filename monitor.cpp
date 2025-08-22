#include "./monitor.h"
#include <iostream>
#include <thread>
#include <chrono>
using std::cout;using std::flush;using std::this_thread::sleep_for;using std::chrono::seconds;

constexpr float TEMP_HIGH=102.0f;
constexpr float TEMP_LOW=95.0f;
constexpr int PULSE_LOW=60;
constexpr int PULSE_HIGH=100;
constexpr float SPO2_LOW=90.0f;
constexpr int ALERT_BLINKS=6;

void showAlertAnimation(){
  for(int i=0;i<ALERT_BLINKS;i++){
    cout<<"\r* "<<flush;
    sleep_for(seconds(1));
    cout<<"\r *"<<flush;
    sleep_for(seconds(1));
  }
  cout<<"\n";
}

int vitalsOk(float temperature,float pulseRate,float spo2){
  if(temperature>TEMP_HIGH||temperature<TEMP_LOW){
    cout<<"Temperature is critical!\n";
    showAlertAnimation();
    return 0;
  }
  if(pulseRate<PULSE_LOW||pulseRate>PULSE_HIGH){
    cout<<"Pulse Rate is out of range!\n";
    showAlertAnimation();
    return 0;
  }
  if(spo2<SPO2_LOW){
    cout<<"Oxygen Saturation is out of range!\n";
    showAlertAnimation();
    return 0;
  }
  return 1;
}
