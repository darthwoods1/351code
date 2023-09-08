#include "initialData.h"


    Counter::Counter(PinName pin) 
        : interrupt(pin), _count(0), _lastPulse(0), _lastState(false), InterruptFlag(false), _startTime(0), _endTime(0)                     //Paramatized constructor calling digital input (reed switch) from D0 in
    {                                                        //member initialization for interrupt
        interrupt.rise(callback(this, &Counter::debounce));  //calling debounce from interrupt for digitial value on rising edge
        debounceTimer.start();                               //starting a debounce timer
        timeSinceLastFlow.start();                           //starting time since last flow timer
        _startTime = std::time(nullptr);                     //store current time in start time 
    }

    void Counter::debounce()                                          //debounce implementation 
    {
        if(debounceTimer.read_ms() >= debounceTime)          //if time between pulse > 10 ms , count as an increment
        {
            increment();
        }
        else                                                //reset debounce timer if not
        {
            debounceTimer.reset();
        }
    }
    void Counter::increment()                                        //increment adds to count
    {
        _count++;
        PulseInterrupt();
        timeSinceLastFlow.reset();
        debounceTimer.reset();
    }

    void Counter::PulseInterrupt()
    {
        InterruptFlag = true;
    }


    int Counter::readCount()
    {
        return _count;
    }
                        //Timers
    int Counter::startTime()
    {
        return _startTime;
    }


    int Counter::readtime()
    {
        return debounceTimer.read_ms();
    }

    void Counter::resetCount()
    {
        _count = 0;
    }

