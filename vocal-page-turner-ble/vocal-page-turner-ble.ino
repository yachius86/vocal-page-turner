/*
  Vocal Page Turner

  Use the Arduino Nano 33 BLE Sense to create a bluetooth HID that emits keystrokes based on voice commands using the built-in bluetooth and microphone.
*/

#include <ArduinoBLE.h>
#include "nano_33_board_utilities.h"

BLEService genericService("1800"); // BLE Generic Access Profile Service
BLEService hidService("1812"); // BLE HID Service
BLEService battService("180F"); // BLE Battery Service
BLEService mfrService("180A"); // BLE Manufacturer Service
BLEUnsignedCharCharacteristic inputCharacteristic("2a4d", BLERead | BLENotify);

nano33BoardUtilities boardUtil;

void setup() {
  Serial.begin(9600);
  while (!Serial);  

  constructHIDKeyboard();
    
  boardUtil.controlRGBLED(1); // white LED
  Serial.println("Ready");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    boardUtil.controlRGBLED(3);

    // while the central is still connected to peripheral:
    while (central.connected()) {      
      if (inputCharacteristic.subscribed()) {
        Serial.println("Subscribed. Writing nonsense.");
//        unsigned char report[8] = { // keydown, capital A
//          0x02, // modifier, Shift
//          0x00, // reserved
//          0x04, // key slot, A
//          0x00, 0x00 ,0x00, 0x00, 0x00 //empty key slots
//        };
//        inputCharacteristic.writeValue((byte)0x02);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x04);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
        //inputCharacteristic.writeValue(report, 8); // send
        
//        unsigned char report[8] = { // keyup
//          0x00, // no modifiers
//          0x00, // reserved
//          0x00, 0x00, 0x00 ,0x00, 0x00, 0x00 // all empty key slots
//        };
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
//        inputCharacteristic.writeValue((byte)0x00);
        //inputCharacteristic.writeValue(report, 8); // send
        
        delay(1000);        
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());

    boardUtil.controlRGBLED(1);
  }
}

void constructHIDKeyboard() {

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setDeviceName("Vocal Page Turner");
  BLE.setLocalName("Arduino Nano 33 BLE Sense");

  BLECharacteristic appearanceCharacteristic("2a00", BLERead, "Keyboard"); // string appearance
  BLECharCharacteristic appearanceHexCharacteristic("2a01", BLERead); // hex appearance, value assigned separately
  BLECharacteristic reportMapCharacteristic("2a4b", BLERead, "05010906A101050719E029E71500250175019508810295017508810395057501050819012905910295017503910395067508150025650507190029658100C0");
  BLECharacteristic controlPointCharacteristic("2a4c", BLEWriteWithoutResponse, NULL, 1);
  BLEUnsignedCharCharacteristic hidInfoCharacteristic("2a4a", BLERead);
  
  BLEDescriptor inputDescriptor("2902", "0001");
  BLEUnsignedCharCharacteristic battCharacteristic("2A19", BLERead | BLENotify);
  BLECharacteristic mfrCharacteristic("2A29", BLERead, "ACohen");
  BLECharacteristic pnpCharacteristic("2A50", BLERead, "014700ffffffff");    

  appearanceHexCharacteristic.writeValue((byte)0x03C2); // present as a keyboard
  hidInfoCharacteristic.writeValue(00010002);
  battCharacteristic.writeValue((byte)0x64); // simulate 100% battery life
  //pnpCharacteristic.writeValue("014700ffffffff");
  inputCharacteristic.addDescriptor(inputDescriptor);

  // add the characteristics to the services
  genericService.addCharacteristic(appearanceCharacteristic);
  genericService.addCharacteristic(appearanceHexCharacteristic);
  hidService.addCharacteristic(reportMapCharacteristic);
  hidService.addCharacteristic(controlPointCharacteristic);
  hidService.addCharacteristic(hidInfoCharacteristic);
  battService.addCharacteristic(battCharacteristic);
  mfrService.addCharacteristic(mfrCharacteristic);
  mfrService.addCharacteristic(pnpCharacteristic);  

  // add the services
  BLE.addService(genericService);
  BLE.addService(hidService);
  BLE.addService(battService);
  BLE.addService(mfrService);

  // start advertising
  BLE.setAdvertisedService(hidService);
  BLE.advertise();

  Serial.println("BLE HID Peripheral");
}
