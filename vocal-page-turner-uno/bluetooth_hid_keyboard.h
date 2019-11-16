/*
  Send HID (keyboard/mouse/etc) data via BLE
*/

#ifndef bluetoothHIDKeyboard_H
#define bluetoothHIDKeyboard_H

#include "Arduino.h"
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         0
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
/*=========================================================================*/

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

class bluetoothHIDKeyboard {
  
  public:
    
    void error(const __FlashStringHelper*err) {
      Serial.println(err);
      while (1);
    };

    void init_hid(void)
    { 
      /* Initialise the module */
      Serial.print(F("Initialising the Bluefruit LE module: "));
    
      if ( !ble.begin(VERBOSE_MODE) )
      {
        error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
      }
      Serial.println( F("OK!") );
    
      if ( FACTORYRESET_ENABLE )
      {
        /* Perform a factory reset to make sure everything is in a known state */
        Serial.println(F("Performing a factory reset: "));
        if ( ! ble.factoryReset() ){
          error(F("Couldn't factory reset"));
        }
      }
    
      /* Disable command echo from Bluefruit */
      ble.echo(false);
    
      Serial.println("Requesting Bluefruit info:");
      /* Print Bluefruit information */
      ble.info();
    
      /* Change the device name to make it easier to find */
      Serial.println(F("Setting device name to 'Vocal Page Turner': "));
      if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=Vocal Page Turner" )) ) {
        error(F("Could not set device name?"));
      }
    
      /* Enable HID Service */
      Serial.println(F("Enable HID Service (including Keyboard): "));
      if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
      {
        if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
          error(F("Could not enable Keyboard"));
        }
      }else
      {
        if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
          error(F("Could not enable Keyboard"));
        }
      }
    
      /* Add or remove service requires a reset */
      Serial.println(F("Performing a SW reset (service changes require a reset): "));
      if (! ble.reset() ) {
        error(F("Couldn't reset??"));
      }
    
      Serial.println();
      Serial.println(F("Go to your phone's Bluetooth settings to pair your device"));
      Serial.println(F("then open an application that accepts keyboard input"));
    
      Serial.println();
      Serial.println(F("Enter the character(s) to send:"));
      Serial.println(F("- \\r for Enter"));
      Serial.println(F("- \\n for newline"));
      Serial.println(F("- \\t for tab"));
      Serial.println(F("- \\b for backspace"));
    
      Serial.println();
    };

    void send_keys(char keys)
    {
      Serial.print("\nSending ");
      Serial.println(keys);

      ble.print("AT+BleKeyboard=");
      ble.println(keys);
      
      ble.waitForOK();
    };

    void send_hid_report(String hidKey)
    {
      //key down
      ble.print("AT+BLEKEYBOARDCODE=");
      ble.print("00-00-");
      ble.print(hidKey);
      ble.println("-00-00-00-00");
      
      //key up      
      ble.println("AT+BLEKEYBOARDCODE=00-00-00-00-00-00-00");
      
      ble.waitForOK();
    };
};

#endif
