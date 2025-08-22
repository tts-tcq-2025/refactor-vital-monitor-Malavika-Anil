#include "./monitor.h"
#include <iostream>
#include <thread>
#include <chrono>
using std::cout;using std::flush;using std::this_thread::sleep_for;using std::chrono::seconds;

constexpr float TEMP_HIGH=102.0f,TEMP_LOW=95.0f;
constexpr int PULSE_LOW=60,PULSE_HIGH=100;
constexpr float SPO2_LOW=90.0f;
constexpr int ALERT_BLINKS=6;

static void showAlert(){
  for(int i=0;i<ALERT_BLINKS;i++){
    cout<<"\r* "<<flush;
    sleep_for(seconds(1));
    cout<<"\r *"<<flush;
    sleep_for(seconds(1));
  }
  cout<<"\n";
}

static bool checkVital(bool condition,const char* message){
  if(condition){cout<<message<<"\n";showAlert();return false;}
  return true;
}

int vitalsOk(float temperature,float pulseRate,float spo2){
  return checkVital(temperature>TEMP_HIGH||temperature<TEMP_LOW,"Temperature is critical!")&&
         checkVital(pulseRate<PULSE_LOW||pulseRate>PULSE_HIGH,"Pulse Rate is out of range!")&&
         checkVital(spo2<SPO2_LOW,"Oxygen Saturation is out of range()");
}
