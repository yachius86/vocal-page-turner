# vocal-page-turner
Arduino-based, voice controlled, bluetooth keyboard simulator to allow turning the pages of an e-book with vocal commands.

The BLE project uses an Arduino Nano 33 BLE Sense and the ArduinoBLE library by Arduino. The voice recognition proved too difficult to build from scratch so this project is incomplete and non-functional. Having all the sensors on one, very small, very low power chip with no wiring or soldering required to get running is appealing for easy distribution, if good voice recognition libraries become available for the Nano's onboard microphone I will revisit this implementation.

The UNO project uses an Arduino UNO R3, the Adafruit BluefruitLE shield and library, and the EasyVR3 shield and library. This is a complete, functional Vocal Page Turner.

The two shields conflict with each other by default, to make the stack work 
* Short the CS pad on the BluefruitLE (pin 8 by default) to pin 6. This requires cutting the factory pad connector.
* Short pin 13 on the EasyVR3 to pin 9
* Short pin 12 on the EasyVR3 to pin 8
This configuration allows the BluefruitLE to use 11, 12, and 13 to communicate with the BLE module and frees up pin 8 for the EasyVR3 to use as a serial bus.

In addition to the code in this repo, EasyVR Commander v3.14.0 was used to add and train commands in Group 1. This code will only listen for words in group 1 and does not need a wake word.

I have Group 1 entry 0 trained as "next page" and Group 1 entry 1 trained as "previous page". Those indexes are mapped to the right arrow and left arrow keys in the simulated keyboard. Left unmodified, this project will only respond to commands in those two slots and will only send those two keystrokes. It should be fairly straightforward to use EasyVR Commander to add custom commands and then map them to arbitrary keystrokes.
