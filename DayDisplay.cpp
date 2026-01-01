#include "DayDisplay.h"
#include <Arduino.h>

DayDisplay::DayDisplay(Adafruit_NeoPixel* neoPixelStrip) {
  strip = neoPixelStrip;
}

void DayDisplay::showDay(Day day) {
  strip->clear();
  
  int pixelIndex = (int)day;
  
  if (pixelIndex >= 0 && pixelIndex < strip->numPixels()) {
    uint32_t color;
    switch (day) {
      case DAY_MONDAY:
      case DAY_WEDNESDAY:
      case DAY_FRIDAY:
        color = strip->Color(0, 0, 150); // Blue
        break;
      case DAY_TUESDAY:
      case DAY_THURSDAY:
      case DAY_SATURDAY:
        color = strip->Color(0, 150, 0); // Green
        break;
      case DAY_SUNDAY:
        color = strip->Color(255, 0, 0); // Red (Max Brightness)
        break;
      default:
        color = strip->Color(0, 0, 0); // Off/Error
        break;
    }
    strip->setPixelColor(pixelIndex, color);
    
    const char* dayNames[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    Serial.print("Today is: ");
    Serial.println(dayNames[pixelIndex]);
  }
  
  strip->show();
}
