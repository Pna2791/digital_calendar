#ifndef WIFICONNECTOR_H
#define WIFICONNECTOR_H

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "config.h"
#include "DoubleDigit.h"

class WifiConnector {
  private:
    const int priorityAddr = 0; // EEPROM address for priority index
    void connectTo(int index);
    DoubleDigit* display; // Pointer to display for progress

  public:
    WifiConnector();
    void setDisplay(DoubleDigit* ptr); // Set display to use
    void connect();
    void check(); // Check connection and reconnect if needed
};
#endif

#endif
