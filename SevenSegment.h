#ifndef SEVENSEGMENT_H
#define SEVENSEGMENT_H

#include <Adafruit_NeoPixel.h>

class SevenSegment {
  private:
    Adafruit_NeoPixel* strip;
    int ledsPerSeg;
    int pin;
    uint32_t _color;

  public:
    SevenSegment(int pin, int ledsPerSegment);
    void begin();
    void showNumber(int number);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(uint32_t c);
};

#endif
