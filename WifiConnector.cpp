#include "WifiConnector.h"

#ifdef ESP8266

WifiConnector::WifiConnector() {
  display = nullptr;
}

void WifiConnector::setDisplay(DoubleDigit* ptr) {
  display = ptr;
}

void WifiConnector::connect() {
  EEPROM.begin(512);
  // ... (rest of connect same as before)
  
  // Read priority index from EEPROM
  int priority = EEPROM.read(priorityAddr);
  
  // Validate priority (must be 0 or 1)
  if (priority != 0 && priority != 1) {
    priority = 0; // Default to first network
  }
  
  // Try connecting to priority network first
  connectTo(priority);
  
  // Check if connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to priority network. Switching...");
    
    // Switch to the other network
    int otherIndex = (priority == 0) ? 1 : 0;
    connectTo(otherIndex);
    
    if (WiFi.status() == WL_CONNECTED) {
      // If successful on the other network, update priority in EEPROM
      Serial.println("Connected to secondary network. Updating priority.");
      EEPROM.write(priorityAddr, otherIndex);
      EEPROM.commit();
    }
  } else {
    Serial.println("Connected to priority network.");
  }
}

void WifiConnector::check() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi connection lost. Attempting to reconnect...");
    connect();
  }
}

void WifiConnector::connectTo(int index) {
  const char* ssid;
  const char* pass;
  
  if (index == 0) {
    ssid = WIFI_SSID_1;
    pass = WIFI_PASS_1;
  } else {
    ssid = WIFI_SSID_2;
    pass = WIFI_PASS_2;
  }
  
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  
  // Wait up to 60 seconds for connection
  unsigned long startAttempt = millis();
  int seconds = 0;
  
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 60000) {
    // Update display if available
    if (display != nullptr) {
      display->setColor(150, 150, 0); // Yellow indicating wait
      display->showNumber(seconds);
    }
    
    delay(1000);
    seconds++;
    Serial.print(".");
  }
  Serial.println();
}

#endif
