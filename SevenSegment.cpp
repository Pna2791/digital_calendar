#include "SevenSegment.h"

// Segment definitions (A-G)
// A = 0
// B = 1
// ...
// G = 6
// Mapping (1 = ON, 0 = OFF) for segments A, B, C, D, E, F, G
const byte numberSegments[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

SevenSegment::SevenSegment(int p, int lps) {
  pin = p;
  ledsPerSeg = lps;
  // Calculate total pixels: 7 segments * ledsPerSegment
  strip = new Adafruit_NeoPixel(7 * ledsPerSeg, pin, NEO_GRB + NEO_KHZ800);
  _color = strip->Color(255, 0, 0); // Default Red
}

void SevenSegment::begin() {
  strip->begin();
  strip->show(); // Initialize all pixels to 'off'
}

void SevenSegment::setColor(uint8_t r, uint8_t g, uint8_t b) {
  _color = strip->Color(r, g, b);
}

void SevenSegment::setColor(uint32_t c) {
  _color = c;
}

// Mapping from Standard Segment Index (A=0 ... G=6) to Physical Strip Order Index
// Physical Order: B, A, F, G, C, D, E
// Index 0 (A) -> Physical Position 1
// Index 1 (B) -> Physical Position 0
// Index 2 (C) -> Physical Position 4
// Index 3 (D) -> Physical Position 5
// Index 4 (E) -> Physical Position 6
// Index 5 (F) -> Physical Position 2
// Index 6 (G) -> Physical Position 3
const int segmentMapping[7] = {1, 0, 4, 5, 6, 2, 3};

void SevenSegment::showNumber(int number) {
  strip->clear();
  
  if (number >= 0 && number <= 9) {
    for (int seg = 0; seg < 7; seg++) {
      // Check if standard segment (A-G) should be on
      if (numberSegments[number][seg] == 1) {
        
        // Find the physical position of this segment
        int physicalSeg = segmentMapping[seg];
        
        // Light up all LEDs in this physical segment
        int startPixel = physicalSeg * ledsPerSeg;
        for (int i = 0; i < ledsPerSeg; i++) {
           strip->setPixelColor(startPixel + i, _color); // Use set color
        }
      }
    }
  } else if (number == -1) {
    // Nothing, already cleared
  }
  
  strip->show();
}
