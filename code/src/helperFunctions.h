
#include "Arduino.h"
#include "FastLED.h"

void fillRing(CRGB color, int ringSizes[], int ringIndex);









void fillRing(CRGB color, int ringSizes[], int ringIndex) {
    int tottalLeds = 0;
    for (int i = 0; i < ringIndex; i++) {
        tottalLeds += ringSizes[i];
    }
    
  fill_solid(leds, ringSizes[ringIndex], color);
}