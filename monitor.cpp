#include "./monitor.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <array>
using std::cout;using std::flush;using std::this_thread::sleep_for;using std::chrono::seconds;

constexpr float TEMP_LOW=95.0f,TEMP_HIGH=102.0f;
constexpr float PULSE_LOW=60.0f,PULSE_HIGH=100.0f;
constexpr float SPO2_MIN=90.0f;
constexpr int OK=1,NOT_OK=0,ALERT_BLINKS=6;

struct Vital{
  bool bad;
  const char* message;
};

static void blinkAlert(){
  bool state=false;
  for(int i=0;i<ALERT_BLINKS*2;i++){
    cout<<"\r"<<(state?"* ":" *")<<flush;
    state=!state;
    sleep_for(seconds(1));
  }
  cout<<"\r  \r"<<flush;
}

int vitalsOk(float temperature,float pulseRate,float spo2){
  bool tempBad=(temperature>TEMP_HIGH)||(temperature<TEMP_LOW);
  bool pulseBad=(pulseRate<PULSE_LOW)||(pulseRate>PULSE_HIGH);
  bool spo2Bad=(spo2<SPO2_MIN);

  std::array<Vital,3> checks{{
    {tempBad,"Temperature is critical!"},
    {pulseBad,"Pulse Rate is out of range!"},
    {spo2Bad,"Oxygen Saturation out of range!"}
  }};

  for(const auto& v:checks){
    if(v.bad){cout<<v.message<<"\n";blinkAlert();return NOT_OK;}
  }
  return OK;
}
