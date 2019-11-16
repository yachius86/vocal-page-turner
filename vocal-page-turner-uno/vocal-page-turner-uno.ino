/*
  Vocal Page Turner

  Use the Arduino Nano 33 BLE Sense to create a bluetooth HID that emits keystrokes based on voice commands using the built-in bluetooth and microphone.
*/

#include "Arduino.h"
#include "bluetooth_hid_keyboard.h"
#include "easyvr_3.h"

EasyVR3 vrUtil;

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println();
  Serial.println("Initializing VPT");
  vrUtil.init_vr();
  Serial.println("VPT Ready");
}

void loop()
{
  int8_t recognized_command_num = vrUtil.vr_listen();

  switch (recognized_command_num) {
    case 0:
      //next page
      Serial.println("Recognized command 0, sending command to keyboard");
      break;
    case 1:
      //previous page
      Serial.println("Recognized command 1, sending command to keyboard");
      break;
    case -1:
      //no recognized command before the timeout or an error
      break;
    default:
      Serial.println("Unrecognized command"); 
  }
}
