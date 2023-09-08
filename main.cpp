/*
 * Copyright (c) 2017-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */
//LoRaWan headers
#include "LoRaWANInterface.h"
#include "lorawan/LoRaWANInterface.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "events/EventQueue.h"

//Application headers
#include "trace_helper.h"
#include "lora_radio_helper.h"
#include "initialData.h"
#include <cstdint>
//Buffers changed for max payload size
uint8_t tx_buffer[35];
uint8_t rx_buffer[35];
// Application dependent transmission timer in ms. Used when duty cycling is off
#define TX_TIMER                        10000
// Max amount of events in event queue
#define MAX_NUMBER_OF_EVENTS            10
// Max number of retries
#define CONFIRMED_MSG_RETRY_COUNTER     3

//event handler
static void lora_event_handler(lorawan_event_t event);

static LoRaWANInterface lorawan(radio);

static lorawan_app_callbacks_t callbacks;

static EventQueue ev_queue(MAX_NUMBER_OF_EVENTS *EVENTS_EVENT_SIZE);





using namespace std;

volatile float volume;
volatile int ID;
volatile bool InterruptFlag = false;
time_t startTime, endTime;

Counter counter(D0);

int main() {
    // genereate ID
    ID = makeID();
    // setup tracing
    setup_trace();
    // stores the status of a call to LoRaWAN protocol
    lorawan_status_t retcode;
    // Initialize LoRaWAN stack
    if (lorawan.initialize(&ev_queue) != LORAWAN_STATUS_OK) {
        printf("\r\n LoRa initialization failed! \r\n");
        return -1;
    }
    printf("\r\n Mbed LoRaWANStack initialized \r\n");
  // prepare application callbacks
    callbacks.events = mbed::callback(lora_event_handler);
    lorawan.add_app_callbacks(&callbacks);

    // Set number of retries in case of CONFIRMED messages
    if (lorawan.set_confirmed_msg_retries(CONFIRMED_MSG_RETRY_COUNTER)
            != LORAWAN_STATUS_OK) {
        printf("\r\n set_confirmed_msg_retries failed! \r\n\r\n");
        return -1;
    }

    printf("\r\n CONFIRMED message retries : %d \r\n",
           CONFIRMED_MSG_RETRY_COUNTER);

    // Enable adaptive data rate
    if (lorawan.enable_adaptive_datarate() != LORAWAN_STATUS_OK) {
        printf("\r\n enable_adaptive_datarate failed! \r\n");
        return -1;
    }

    printf("\r\n Adaptive data  rate (ADR) - Enabled \r\n");

    retcode = lorawan.connect();

    if (retcode == LORAWAN_STATUS_OK ||
            retcode == LORAWAN_STATUS_CONNECT_IN_PROGRESS) {
    } else {
        printf("\r\n Connection error, code = %d \r\n", retcode);
        return -1;
    }

    printf("\r\n Connection - In Progress ...\r\n");

    // make your event queue dispatching events forever
    ev_queue.dispatch_forever();




return 0;
}






static void send_message()
{

 uint16_t packet_len;
    int16_t retcode;
    int32_t count;
    std:vector<data> dataVector;
while(counter.readtime() >= 5)
    {
    if (InterruptFlag) {                    // InterruptFlag
      volatile int count;                   // holder variable for count DELETE
      count = counter.readCount();          // set holder variable DELETE
      cout << "Count: " << counter.readCount()
           << "\n"                          // print to terminal every cycle
           << "Volume: " << count * CALIBRATIONFACTOR;
      cout << "Time since flow: " << counter.readtime() << endl;
    } else {
        printf("\r\n No data \r\n");
        return;
    }
    }

        std::string startTimeString = convertTime(counter.startTime()); //Conversion of start and end time to string to allow for print DELETE
        std::string endTimeString = convertTime(std::time(nullptr));

                                                                        //print to terminal final values DELETE
        cout << "\nFinished Counting device:\n\n"
             << "Final volume " << count * CALIBRATIONFACTOR
        cout << " Litres \n Start and end times: " << startTimeString << ", "
             << endTimeString << "\n\n";
        struct data structPacket;           // create instance of data, current event data
        structPacket.flag = exclusionFlag();
        structPacket.Volume = counter.readCount() * CALIBRATIONFACTOR;
        structPacket.count = counter.readCount();
        structPacket.ID = makeID();
        structPacket.time = endTime;
        structPacket.lat = 211.253;
        structPacket.lon = -321.321;
        dataVector.push_back(structPacket);//push back current packet, for record of past MODIFY   
                                           //Print to terminal past events
        cout << "TESTING STRUCTS" << endl;
        int i = 0;
        for (const auto &structPacket : dataVector) {
          if (volume)
            cout << "Number of event: " << i << "  ID: " << structPacket.ID
                 << ", Volume: " << structPacket.Volume << std::endl;
          i++;
        }
                                            
        std::vector<uint8_t> serializedData = serializeData(structPacket);  //serialize current data packet for trasmission
                                                                            
        std::cout << "Serialized Data Buffer:" << std::endl;                //print to terminal bytes of data packet
        for (uint8_t byte : serializedData) {
          std::cout << std::bitset<8>(byte) << " ";
        }


    
    //packet_len = ;

        for (uint8_t byte : serializedData) {
             uint8_t byteSend = static_cast <uint8_t> (byte);
          retcode = lorawan.send(MBED_CONF_LORA_APP_PORT, &byteSend, sizeof(byteSend),
                           MSG_UNCONFIRMED_FLAG);
        }

    if (retcode < 0) {
        retcode == LORAWAN_STATUS_WOULD_BLOCK ? printf("send - WOULD BLOCK\r\n")
        : printf("\r\n send() - Error code %d \r\n", retcode);

        if (retcode == LORAWAN_STATUS_WOULD_BLOCK) {
            //retry in 3 seconds
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                ev_queue.call_in(3000, send_message);
            }
        }


        counter.resetCount();
        volume = 0;
        count = 0;
        startTime = 0;
        endTime = 0;
        InterruptFlag = false;
        return;
    }

        // reset all variables and counter class count  DO IN COUNTER CLASS
        counter.resetCount();
        volume = 0;
        count = 0;
        startTime = 0;
        endTime = 0;
        InterruptFlag = false;

        ThisThread::sleep_for(2000);  
      }



/**
 * Event handler
 */
static void lora_event_handler(lorawan_event_t event)
{
    switch (event) {
        case CONNECTED:
            printf("\r\n Connection - Successful \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            } else {
                ev_queue.call_every(TX_TIMER, send_message);
            }

            break;
        case DISCONNECTED:
            ev_queue.break_dispatch();
            printf("\r\n Disconnected Successfully \r\n");
            break;
        case TX_DONE:
            printf("\r\n Message Sent to Network Server \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        case TX_TIMEOUT:
        case TX_ERROR:
        case TX_CRYPTO_ERROR:
        case TX_SCHEDULING_ERROR:
            printf("\r\n Transmission Error - EventCode = %d \r\n", event);
            // try again
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        case RX_TIMEOUT:
        case RX_ERROR:
            printf("\r\n Error in reception - Code = %d \r\n", event);
            break;
        case JOIN_FAILURE:
            printf("\r\n OTAA Failed - Check Keys \r\n");
            break;
        case UPLINK_REQUIRED:
            printf("\r\n Uplink required by NS \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        default:
            MBED_ASSERT("Unknown Event");
    }
}

// EOF
