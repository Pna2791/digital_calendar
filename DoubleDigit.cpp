#include "DoubleDigit.h"

DoubleDigit::DoubleDigit(SevenSegment* tensSeg, SevenSegment* onesSeg) {
  tens = tensSeg;
  ones = onesSeg;
}

void DoubleDigit::showNumber(int number) {
  if (number < 0) {
    tens->showNumber(-1); // Off
    ones->showNumber(-1); // Off
  } else {
    // Ensure number is within 0-99 range effectively for display
    int val = number % 100;
    
    int tensDigit = val / 10;
    int onesDigit = val % 10;
    
    // Hide leading zero if tensDigit is 0 (except if the number is literally 0? No, user said do not show 0 as FIRST digit)
    // Interpreting "first digit" as the tens place.
    // If number is 0, we usually show "0". So tens is blank, ones is 0. 
    // If number is 5, we show " 5".
    if (tensDigit == 0) {
      tens->showNumber(-1); // Blank
    } else {
      tens->showNumber(tensDigit);
    }
    ones->showNumber(onesDigit);
  }
}

void DoubleDigit::setColor(uint8_t r, uint8_t g, uint8_t b) {
  tens->setColor(r, g, b);
  ones->setColor(r, g, b);
}

void DoubleDigit::setColor(uint32_t c) {
  tens->setColor(c);
  ones->setColor(c);
}
