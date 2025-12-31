#ifndef DOUBLEDIGIT_H
#define DOUBLEDIGIT_H

#include "SevenSegment.h"

class DoubleDigit {
  private:
    SevenSegment* tens;
    SevenSegment* ones;

  public:
    DoubleDigit(SevenSegment* tensSeg, SevenSegment* onesSeg);
    void showNumber(int number);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(uint32_t c);
};

#endif
