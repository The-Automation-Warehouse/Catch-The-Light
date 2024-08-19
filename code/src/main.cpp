#include <Arduino.h>

#include <FastLED.h>

#define NUM_LEDS 300
#define DATA_PIN 3

CRGB leds[NUM_LEDS];

void setup() { 

    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(20);

}

void loop() { 
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(200);
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(200);
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  delay(200);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(500);
}