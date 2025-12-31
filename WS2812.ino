#include <Adafruit_NeoPixel.h>
#include "DayDisplay.h"
#include "SevenSegment.h"
#include "DoubleDigit.h"

// Pin Definitions
#if defined(ESP8266)
  #include "WifiConnector.h" // Include Wi-Fi logic
  #define PIN_DAY    D7
  #define PIN_SEG1   D6
  #define PIN_SEG2   D5
  #define PIN_SEG3   D2
  #define PIN_SEG4   D1
#else
  #define PIN_DAY    3
  #define PIN_SEG1   4
  #define PIN_SEG2   5
  #define PIN_SEG3   6
  #define PIN_SEG4   7
#endif

#define NUMPIXELS  7

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_DAY, NEO_GRB + NEO_KHZ800);

DayDisplay* dayDisp;
SevenSegment* seg1;
SevenSegment* seg2;
SevenSegment* seg3;
SevenSegment* seg4;
DoubleDigit* numDisplay1;
DoubleDigit* numDisplay2;

#if defined(ESP8266)
WifiConnector connector;
#include <ArduinoOTA.h>
#endif

#include <time.h>
#include "timestampe_to_lunar_date.h"

// Helper function to set display colors based on requirements
void setDisplayColors() {
  // NumDisplay1 (Lunar) - Let's use Cyan? Or keep Green?
  // NumDisplay2 (Solar) - Let's use Orange?
  // For now relying on default or loop setting. 
}

// Day names for Serial logging
#define utc_7 25200
const char* dayNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

void showDate(uint32_t timestamp) {
  // Use a copy of timestamp for display logic if needed, but localtime handles it.
  // timestamp is already UTC (from NTP or base). 
  // We need to add UTC+7 for display/calculation purposes as per previous logic
  time_t rawTime = (time_t)(timestamp + utc_7);
  struct tm* timeInfo = localtime(&rawTime);
  
  // 1. Calculate Day of Week
  // tm_wday: 0=Sun, 1=Mon, ..., 6=Sat
  // Our Day Enum: DAY_MONDAY=0 ... DAY_SUNDAY=6
  int wday = timeInfo->tm_wday;
  Day currentDay = (Day)((wday + 6) % 7);
  
  // 2. Solar Day
  int solarDay = timeInfo->tm_mday;
  
  // 3. Lunar Day
  int lunarDay = getLunarDay(timestamp + utc_7); // lunar func expects timestamp in target timezone? 
  // Check helper: "Timestamp của 01/12/2025 00:00:00 UTC" -> it compares diff.
  // Let's stick to passing the adjusted timestamp if getLunarDay expects local representation or consistent UTC.
  // The helper uses EPOCH_01_12_2025 (UTC).
  // If we pass (timestamp + utc_7), we are shifting it.
  // Let's assume getLunarDay expects the same time basis as the visual date (Local).
  // Actually, let's verify if getLunarDay needs raw UTC or Local.
  // The user script generated mapping based on 01/12/2025.
  // Let's pass the same timestamp we used for solar calculation: (timestamp + utc_7) 
  // Wait, if I pass (timestamp + utc_7) to getLunarDay, and getLunarDay subtracts a UTC EPOCH, the diff will be shifted.
  // Let's keep consistent with previous approved logic: `getLunarDay(timestamp)` where timestamp was passed into showDate.
  // But wait, in `showDate` passed `testTimestamp` was `1770508799`.
  // The new logic passes `now` (UTC).
  // So inside `showDate`, we shift for Solar `timestamp += utc_7`.
  // `getLunarDay` should probably take the shifted timestamp if it's based on local day boundaries, OR standardized UTC.
  // Given previous usage: `getLunarDay(timestamp)` was called *before* `timestamp += utc_7` in previous diff?
  // No, `timestamp += utc_7` was at top of function.
  // So `getLunarDay` received the shifted timestamp.
  
  // LOGGING
  char timeStr[64];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeInfo);
  Serial.print(timeStr);
  Serial.print(" ");
  Serial.print(dayNames[currentDay]); // 0-6 index matches array
  Serial.print(", Lunar: ");
  Serial.println(lunarDay);
  
  // Update Displays
  dayDisp->showDay(currentDay);
  
  if (lunarDay != -1) {
    numDisplay1->setColor(0, 0, 150); // Blue for Lunar
    numDisplay1->showNumber(lunarDay);
  } else {
    numDisplay1->showNumber(-1); // Error/Out of range
  }
  
  numDisplay2->setColor(150, 150, 0); // Yellow/Orange for Solar
  numDisplay2->showNumber(solarDay);
}

void setup() {
  Serial.begin(9600);
  
  pixels.begin();
  dayDisp = new DayDisplay(&pixels);
  
  // Initialize Seven Segments
  seg1 = new SevenSegment(PIN_SEG1, 2);
  seg2 = new SevenSegment(PIN_SEG2, 2);
  seg3 = new SevenSegment(PIN_SEG3, 3);
  seg4 = new SevenSegment(PIN_SEG4, 3);
  
  seg1->begin();
  seg2->begin();
  seg3->begin();
  seg4->begin();
  
  // Initialize Double Digits
  // Pin 4/5 -> Display 1
  numDisplay1 = new DoubleDigit(seg2, seg1);
  
  // Pin 6/7 -> Display 2
  numDisplay2 = new DoubleDigit(seg4, seg3);

  #if defined(ESP8266)
  // Connect to Wi-Fi with progress display
  connector.setDisplay(numDisplay2); // Show count on Display 2
  connector.connect(); 
  configTime(0, 0, "pool.ntp.org", "time.nist.gov"); // Sync time (UTC)
  
  // OTA Configuration
  ArduinoOTA.setHostname("Derichs-Clock");
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    // NOTE: if updating FS this would be the place to unmount FS using SPIFFS.end()
    Serial.println("Start updating " + type);
    numDisplay2->setColor(0, 150, 150); // Cyan
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    numDisplay2->showNumber(100);
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    int percent = (progress / (total / 100));
    numDisplay2->showNumber(percent);
    // Serial.printf("Progress: %u%%\r", percent);
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    numDisplay2->setColor(255, 0, 0); // Red
    numDisplay2->showNumber(error);
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  
  ArduinoOTA.begin();
  #endif
}

unsigned long lastUpdate = 0;

// Default timestamp for non-ESP boards (e.g., 01/01/2026 00:00:00 UTC)
// You can update this to a more recent date before compiling for non-ESP.
#if !defined(ESP8266)
uint32_t baseTimestamp = 1767225600; 
#endif

void loop() {
  uint32_t currentTimestamp = 0;

  #if defined(ESP8266)
    ArduinoOTA.handle(); // Handle OTA updates
    
    // ESP8266 Logic: Use NTP
    if (millis() - lastUpdate > 60000 || lastUpdate == 0) {
      // Check Wi-Fi connection
      connector.check();
      
      time_t now = time(nullptr);
      if (now > 100000) {
        showDate((uint32_t)now);
        lastUpdate = millis();
      }
    }
    delay(100);
  #else
    // Non-ESP Logic: Simulate Time
    // Calculate seconds passed since boot (millis() / 1000)
    currentTimestamp = baseTimestamp + (millis() / 1000);
    
    // Update display every second (or minute if preferred)
    showDate(currentTimestamp);
    
    delay(1000); 
  #endif
}
