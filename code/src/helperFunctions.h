
#include "Arduino.h"
#include "FastLED.h"

void fillRing(CRGB color, int ringSizes[], int ringIndex);









void fillRing(CRGB leds[], CRGB color, int ringSizes[], int ringIndex) {
    int tottalLeds = 0;
    for (int i = 0; i < ringIndex+1; i++) {
        tottalLeds += ringSizes[i];
    }
    
  for (int i = tottalLeds - ringSizes[ringIndex]; i < tottalLeds; i++) {
    leds[i] = color;
  }
}