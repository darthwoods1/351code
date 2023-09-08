// initialData.h
#pragma once

#include "mbed.h"
#include "time.h"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include "Timer.h"
#include <bitset>
#define CALIBRATIONFACTOR 3231;

struct data {
  bool flag;
  int ID;
  int count;
  float Volume;
  time_t time;
  double lat;
  double lon;
};

std::vector<uint8_t> serializeData(const data &d);

int makeID(void);

std::string convertTime(time_t time);

bool exclusionFlag();



class Counter {
private:
  InterruptIn interrupt; // triggering event when digital input changes
  Timer debounceTimer;
  Timer timeSinceLastFlow;
  static const int debounceTime = 10; // milli seconds
  static const int finalTime = 1000;  // milli seconds
  volatile int _count;
  volatile float _lastPulse;
  bool _lastState, InterruptFlag;
  time_t _startTime, _endTime;

public:

  Counter(PinName pin);// Paramatized constructor calling digital input (reed D0)

  void debounce(); // debounce implementation

  void increment(); // increment adds to count

  void PulseInterrupt();

  int readCount();

  // Timers
  int startTime();

  int readtime();

  void resetCount();
};
