/*
  Vocal Page Turner

  Use the Arduino Nano 33 BLE Sense to create a bluetooth HID that emits keystrokes based on voice commands using the built-in bluetooth and microphone.
*/

#include "Arduino.h"
#include "easyvr_3.h"
#include "bluetooth_hid_keyboard.h"

EasyVR3 vrUtil;
bluetoothHIDKeyboard hidUtil;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Initializing VPT");  

  // initialize the keyboard first so it has time to pair
  hidUtil.init_hid();

  // the arduino seems to need a short delay between shield initialization
  delay(150);

  // initialize voice recognition
  vrUtil.init_vr();  
  
  Serial.println("VPT Ready");
}

void loop()
{      
  // listen for a recognized voice command
  int8_t recognized_command_num = vrUtil.vr_listen();

  switch (recognized_command_num) {
    case 0:
      // next page
      Serial.println("Sending command to keyboard");
      // right arrow keystroke
      hidUtil.send_hid_report("4F");
      break;
    case 1:
      // previous page
      Serial.println("Sending command to keyboard");
      // left arrow keystroke
      hidUtil.send_hid_report("50");      
      break;
    case -1:
      // no recognized command before the timeout or an error
      break;
    default:
      // a command the EasyVR3 recognizes that is not meaningful to this program
      Serial.println("Unrecognized command"); 
  }
}
