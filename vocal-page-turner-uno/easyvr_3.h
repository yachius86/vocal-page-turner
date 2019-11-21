/**
  EasyVR

  'm1' - set mic distance to HEADSET
  'm2' - set mic distance to ARMS_LENGTH (default)
  'm3' - set mic distance to FAR_MIC

  With EasyVR Shield, the green LED is ON while the module
  is listening (using pin IO1 of EasyVR).
  Successful recognition is acknowledged with a beep.
  Details are displayed on the serial monitor window.
*/

#ifndef easyVR3_H
#define easyVR3_H

#include "Arduino.h"
#include "EasyVR.h"

#if !defined(SERIAL_PORT_MONITOR)
  #error "Arduino version not supported. Please update your IDE to the latest version."
#endif

#if defined(__SAMD21G18A__)
  // Shield Jumper on HW (for Zero, use Programming Port)
  #define port SERIAL_PORT_HARDWARE
  #define pcSerial SERIAL_PORT_MONITOR
#elif defined(SERIAL_PORT_USBVIRTUAL)
  // Shield Jumper on HW (for Leonardo and Due, use Native Port)
  #define port SERIAL_PORT_HARDWARE
  #define pcSerial SERIAL_PORT_USBVIRTUAL
#else
  // Shield Jumper on SW (using pins 12/13 or 8/9 as RX/TX)
  #include "SoftwareSerial.h"
  SoftwareSerial port(8, 9);
  #define pcSerial SERIAL_PORT_MONITOR
#endif

EasyVR easyvr(port);

const char* ws0[] =
{
  "ROBOT",
};
const char* ws1[] =
{
  "ACTION",
  "MOVE",
  "TURN",
  "RUN",
  "LOOK",
  "ATTACK",
  "STOP",
  "HELLO",
};
const char* ws2[] =
{
  "LEFT",
  "RIGHT",
  "UP",
  "DOWN",
  "FORWARD",
  "BACKWARD",
};
const char* ws3[] =
{
  "ZERO",
  "ONE",
  "TWO",
  "THREE",
  "FOUR",
  "FIVE",
  "SIX",
  "SEVEN",
  "EIGHT",
  "NINE",
  "TEN",
};
const char** ws[] = { ws0, ws1, ws2, ws3 };

class EasyVR3 {
  
  int8_t bits = 4;
  int8_t set = 0;
  int8_t group = 1;
  uint32_t mask = 0;
  uint8_t train = 0;
  int8_t grammars = 0;
  int8_t lang = 0;
  char name[33];
  bool useCommands = true;
  bool useGrammars = false;
  bool useTokens = false;
  bool isSleeping = false;
  bool isBusy = false;
  
  public:
  
    void init_vr() {
      // setup PC serial port
      pcSerial.begin(9600);
      bridge:
      // bridge mode?
      int mode = easyvr.bridgeRequested(pcSerial);
      switch (mode)
      {
      case EasyVR::BRIDGE_NONE:
        // setup EasyVR serial port
        port.begin(9600);
        // run normally
        pcSerial.println();
        pcSerial.println(F("Bridge not requested, run normally"));
        pcSerial.println(F("---"));
        break;
        
      case EasyVR::BRIDGE_NORMAL:
        // setup EasyVR serial port (low speed)
        port.begin(9600);
        // soft-connect the two serial ports (PC and EasyVR)
        easyvr.bridgeLoop(pcSerial);
        // resume normally if aborted
        pcSerial.println();
        pcSerial.println(F("Bridge connection aborted"));
        pcSerial.println(F("---"));
        break;
        
      case EasyVR::BRIDGE_BOOT:
        // setup EasyVR serial port (high speed)
        port.begin(115200);
        pcSerial.end();
        pcSerial.begin(115200);
        // soft-connect the two serial ports (PC and EasyVR)
        easyvr.bridgeLoop(pcSerial);
        // resume normally if aborted
        pcSerial.println();
        pcSerial.println(F("Bridge connection aborted"));
        pcSerial.println(F("---"));
        break;
      }
    
      // initialize EasyVR  
      while (!easyvr.detect())
      {
        pcSerial.println(F("EasyVR not detected!"));
        for (int i = 0; i < 10; ++i)
        {
          if (pcSerial.read() == EasyVR::BRIDGE_ESCAPE_CHAR)
            goto bridge;
          delay(100);
        }
      }
    
      pcSerial.print(F("EasyVR detected, version "));
      pcSerial.print(easyvr.getID());
    
      if (easyvr.getID() < EasyVR::EASYVR3)
        easyvr.setPinOutput(EasyVR::IO1, LOW); // Shield 2.0 LED off
    
      if (easyvr.getID() < EasyVR::EASYVR)
        pcSerial.print(F(" = VRbot module"));
      else if (easyvr.getID() < EasyVR::EASYVR2)
        pcSerial.print(F(" = EasyVR module"));
      else if (easyvr.getID() < EasyVR::EASYVR3)
        pcSerial.print(F(" = EasyVR 2 module"));
      else
        pcSerial.print(F(" = EasyVR 3 module"));
      pcSerial.print(F(", FW Rev."));
      pcSerial.println(easyvr.getID() & 7);
    
      easyvr.setDelay(0); // speed-up replies
    
      if (easyvr.getID() >= EasyVR::EASYVR3_1)
      {
        if (!easyvr.checkMessages() && easyvr.getError() == EasyVR::ERR_CUSTOM_INVALID)
        {
          pcSerial.print(F("Message recovery needed, please wait..."));
          if (easyvr.fixMessages())
            pcSerial.println(F(" done!"));
          else
            pcSerial.println(F(" failed!"));
        }
      }
    
      easyvr.setTimeout(30);
      lang = EasyVR::ENGLISH;
      easyvr.setLanguage(lang);
      // use fast recognition
      easyvr.setTrailingSilence(EasyVR::TRAILING_MIN);
      easyvr.setCommandLatency(EasyVR::MODE_FAST);
    
      int16_t count = 0;
    
      if (easyvr.getGroupMask(mask))
      {
        uint32_t msk = mask;
        for (group = 0; group <= EasyVR::PASSWORD; ++group, msk >>= 1)
        {
          if (!(msk & 1)) continue;
          if (group == EasyVR::TRIGGER)
            pcSerial.print(F("Trigger: "));
          else if (group == EasyVR::PASSWORD)
            pcSerial.print(F("Password: "));
          else
          {
            pcSerial.print(F("Group "));
            pcSerial.print(group);
            pcSerial.print(F(" has "));
          }
          count = easyvr.getCommandCount(group);
          pcSerial.print(count);
          if (group == 0)
            pcSerial.println(F(" trigger(s)"));
          else
            pcSerial.println(F(" command(s)"));
          for (int8_t idx = 0; idx < count; ++idx)
          {
            if (easyvr.dumpCommand(group, idx, name, train))
            {
              pcSerial.print(idx);
              pcSerial.print(F(" = "));
              pcSerial.print(name);
              pcSerial.print(F(", Trained "));
              pcSerial.print(train, DEC);
              if (!easyvr.isConflict())
                pcSerial.println(F(" times, OK"));
              else
              {
                int8_t confl = easyvr.getWord();
                if (confl >= 0)
                  pcSerial.print(F(" times, Similar to Word "));
                else
                {
                  confl = easyvr.getCommand();
                  pcSerial.print(F(" times, Similar to Command "));
                }
                pcSerial.println(confl);
              }
            }
          }
        }
      }
      
      easyvr.stop();
      easyvr.setMicDistance(1);
      pcSerial.println(F("Set mic distance to 'headset'"));
      
      group = 1;
      set = 0;
      mask |= 1; // force to use trigger (mixed SI/SD)
      useCommands = (mask != 0);
      isSleeping = false;
      pcSerial.println(F("---"));
    };
    
    int readNum()
    {
      int rx;
      int num = 0;
      delay(5);
      while ((rx = pcSerial.read()) >= 0)
      {
        delay(5);
        if (isdigit(rx))
          num = num * 10 + (rx - '0');
        else
          break;
      }
      return num;
    };
    
    bool checkMonitorInput()
    {
      if (pcSerial.available() <= 0)
        return false;
    
      // check console commands
      int16_t rx = pcSerial.read();
      if (rx == EasyVR::BRIDGE_ESCAPE_CHAR)
      {
        setup();
        return true;
      }
      if (isSleeping)
      {
        // any character received will exit sleep
        isSleeping = false;
        easyvr.stop();
        pcSerial.println(F("Forced wake-up!"));
        return true;
      }
      if (rx == 'm')
      {
        int16_t num = readNum();
        pcSerial.print(F("Mic distance "));
        pcSerial.println(num);
        easyvr.stop();
        easyvr.setMicDistance(num);
      }
      if (rx >= 0)
      {
        easyvr.stop();
        pcSerial.flush();
        return true;
      }
      return false;
    };
    
    int8_t vr_listen()
    {
      int8_t recognized_command_num = -1;
      checkMonitorInput();
    
      if (!isSleeping && !isBusy)
      {
        if (easyvr.getID() < EasyVR::EASYVR3)
          easyvr.setPinOutput(EasyVR::IO1, HIGH); // Shield 2.0 LED on (listening)
    
        if (useCommands)
        {
          pcSerial.print(F("Say a command in Group "));
          pcSerial.println(group);
          easyvr.recognizeCommand(group);
        }
      }
      do
      {
        if (checkMonitorInput())
          return recognized_command_num;
      }
      while (!easyvr.hasFinished());
      isSleeping = false;
      isBusy = false;
    
      if (easyvr.getID() < EasyVR::EASYVR3)
        easyvr.setPinOutput(EasyVR::IO1, LOW); // Shield 2.0 LED off
    
      if (easyvr.isAwakened())
      {
        pcSerial.println(F("Audio wake-up!"));
        return recognized_command_num;
      }
    
      int16_t idx;
      if (useTokens)
      {
        idx = easyvr.getToken();
        if (idx >= 0)
        {
          pcSerial.print(F("Token: "));
          pcSerial.println(idx);
          easyvr.playSound(0, EasyVR::VOL_FULL);
        }
      }
      // handle voice recognition
      idx = easyvr.getWord();
      if (idx >= 0)
      {
        pcSerial.print(F("Word: "));
        pcSerial.print(easyvr.getWord());
        pcSerial.print(F(" = "));
        if (useCommands)
          pcSerial.println(ws[group][idx]);
        // --- optional: builtin words can be retrieved from the module
        else if (set < 4)
          pcSerial.println(ws[set][idx]);
        // ---
        else
        {
          uint8_t flags, num;
          if (easyvr.dumpGrammar(set, flags, num))
            while (idx-- >= 0)
            {
              if (!easyvr.getNextWordLabel(name))
                break;
            }
          if (idx < 0)
            pcSerial.println(name);
          else
            pcSerial.println();
        }
        // ok, let's try another set
        if (set < 4)
        {
          set++;
          if (set > 3)
            set = 0;
        }
        else
        {
          set++;
          if (set >= grammars)
            set = 4;
        }
        easyvr.playSound(0, EasyVR::VOL_FULL);
      }
      else
      {
        idx = easyvr.getCommand();
        if (idx >= 0)
        {
          pcSerial.print(F("Command: "));
          pcSerial.print(easyvr.getCommand());
          if (easyvr.dumpCommand(group, idx, name, train))
          {
            pcSerial.print(F(" = "));
            pcSerial.println(name);
            recognized_command_num = idx;
          }
          else
            pcSerial.println();
          while (!((mask >> group) & 1));
          easyvr.playSound(0, EasyVR::VOL_FULL);
        }
        else // errors or timeout
        {
          int16_t err = easyvr.getError();
          if (err >= 0)
          {
            pcSerial.print(F("Error 0x"));
            pcSerial.println(err, HEX);
          }
          else if (easyvr.isTimeout())
            pcSerial.println(F("Timed out."));
          else
            pcSerial.println(F("Done."));
        }
      }
      return recognized_command_num;
    };
};

#endif
