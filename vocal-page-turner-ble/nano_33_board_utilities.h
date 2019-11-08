/*
  nano_33_board_utilities.h - Library for controlling board functions of an Arduino Nano 33 BLE Sense
  Created by Aaron Cohen, November 8, 2019.
  Released into the public domain.
*/
#ifndef nano33BoardUtilities_H
#define nano33BoardUtilities_H

#include "Arduino.h"

class nano33BoardUtilities {
  const int ledPin1 = 22; // R
  const int ledPin2 = 23; // G
  const int ledPin3 = 24; // B
  
  public:
    nano33BoardUtilities() {
      //init RGB LED
      pinMode(ledPin1, OUTPUT);
      pinMode(ledPin2, OUTPUT);
      pinMode(ledPin3, OUTPUT);
    };
    
    void controlRGBLED(int color) {
      switch(color) {
        case 1: // white
          digitalWrite(ledPin1, LOW);
          digitalWrite(ledPin2, LOW);
          digitalWrite(ledPin3, LOW);
          break;
        case 2: // red
          digitalWrite(ledPin1, LOW);
          digitalWrite(ledPin2, HIGH);
          digitalWrite(ledPin3, HIGH);
          break;
        case 3: // blue
          digitalWrite(ledPin1, HIGH);
          digitalWrite(ledPin2, LOW);
          digitalWrite(ledPin3, LOW);
          break;
        default:
          // off
          digitalWrite(ledPin1, HIGH);
          digitalWrite(ledPin2, HIGH);
          digitalWrite(ledPin3, HIGH);
      }
  };    
};

#endif
