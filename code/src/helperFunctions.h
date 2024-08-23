
#include "Arduino.h"
#include "FastLED.h"

void fillRing(CRGB color, int ringSizes[], int ringIndex);









void fillRing(CRGB leds[], CRGB color, int ringSizes[], int ringIndex) {
    int tottalLeds = 0;
    for (int i = 0; i < ringIndex+1; i++) {
        tottalLeds += ringSizes[i];
    }
    
  fill_solid(leds, tottalLeds, color);
  fill_solid(leds, tottalLeds - ringSizes[ringIndex], CRGB::Black);
}