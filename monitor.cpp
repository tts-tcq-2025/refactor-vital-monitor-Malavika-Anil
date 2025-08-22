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

int checkAndAlert(bool condition,const char* message){
  if(condition){
    cout<<message<<"\n";
    showAlertAnimation();
    return 0;
  }
  return 1;
}

int vitalsOk(float temperature,float pulseRate,float spo2){
  if(!checkAndAlert(temperature>TEMP_HIGH||temperature<TEMP_LOW,"Temperature is critical!"))return 0;
  if(!checkAndAlert(pulseRate<PULSE_LOW||pulseRate>PULSE_HIGH,"Pulse Rate is out of range!"))return 0;
  if(!checkAndAlert(spo2<SPO2_LOW,"Oxygen Saturation is out of range!"))return 0;
  return 1;
}
