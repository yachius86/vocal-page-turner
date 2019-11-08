/*
  voicekeys

  Use the Arduino Nano 33 BLE Sense to create a bluetooth HID that emits keystrokes based on voice commands using the built-in bluetooth and microphone.
*/

#include <ArduinoBLE.h>

BLEService genericService("1800"); // BLE Generic Access Profile Service
BLECharacteristic appearanceCharacteristic("2a00", BLERead, "Keyboard");
BLEUnsignedCharCharacteristic appearanceHexCharacteristic("2a01", BLERead);

BLEService hidService("1812"); // BLE HID Service
BLECharacteristic reportMapCharacteristic("2a4c", BLERead, "05010906A1010507850119E029E715002501750195088102950175088101950575010508850119012905910295017503910395067508150025650507190029658100C0");
BLECharacteristic controlPointCharacteristic("2a4c", BLEWriteWithoutResponse, NULL, 1);
BLEUnsignedIntCharacteristic hidInfoCharacteristic("2a4a", BLERead);
BLECharacteristic inputCharacteristic("2a4d", BLENotify, NULL);
BLEDescriptor inputDescriptor("2902", "0001");

BLEService battService("180F"); // BLE Battery Service
BLEUnsignedCharCharacteristic battCharacteristic("2A19", BLERead | BLENotify);

BLEService mfrService("180A"); // BLE Manufacturer Service
BLECharacteristic mfrCharacteristic("2A29", BLERead, "ACohen");
BLEUnsignedShortCharacteristic pnpCharacteristic("2A50", BLERead);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setDeviceName("Vocal Page Turner");
  BLE.setLocalName("Arduino Nano 33 BLE Sense");

  appearanceHexCharacteristic.writeValue((byte)0x03C2);
  hidInfoCharacteristic.writeValue(00010002);
  battCharacteristic.writeValue((byte)0x64);
  pnpCharacteristic.writeValue(004700);

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

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
        
      //if (keyboardCharacteristic.subscribed()) {
        Serial.println("Subscribed. Writing nonsense.");
        //keyboardCharacteristic.writeValue(0x04);
        //keyboardCharacteristic.writeValue(0x05);
        
//        if (keyboardCharacteristic.value()) {   // any value other than 0
//          Serial.println(F("LED on"));
//        } else {                              // a 0 value
//          Serial.println(F("LED off"));
//          
//        }
      //}
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}