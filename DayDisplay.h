#ifndef DAYDISPLAY_H
#define DAYDISPLAY_H

#include <Adafruit_NeoPixel.h>

enum Day {
  DAY_MONDAY = 0,
  DAY_TUESDAY,
  DAY_WEDNESDAY,
  DAY_THURSDAY,
  DAY_FRIDAY,
  DAY_SATURDAY,
  DAY_SUNDAY
};

class DayDisplay {
  private:
    Adafruit_NeoPixel* strip;

  public:
    DayDisplay(Adafruit_NeoPixel* neoPixelStrip);
    void showDay(Day day);
};

#endif
