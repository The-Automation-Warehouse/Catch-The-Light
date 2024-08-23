/*
 * Catch-The-Light
 * This is a crude arcade game inspired by "Speed Demon".
  * It consists of a ring of LEDs through which a light travels.
  * In each round, the light travels through the ring and the player has to press a button at the right time to stop the light.
  * If the player presses the button at the right time, the light progresses to the next round and the next ring of LEDs lights up.
  * If the player presses the button at the wrong time, the game is over.
  * The game is won when the light has traveled through all the rings.
  * There are two backlit signs that light up when the game is won or lost.
 */



#include <Arduino.h>
#include <FastLED.h>
#include helperFunctions.h

#define BUTTON 2
#define LEDS_PIN 3
#define BUTTON_LED 4
#define BUZZER 9
#define WIN_LEDS 5
#define TRY_AGAIN_LEDS 6

// Ring 1 - 60 LEDs
// Ring 2 - 48 LEDs
// Ring 3 - 32 LEDs
// Ring 4 - 26 LEDs
// Ring 5 - 16 LEDs
// Ring 6 - 12 LEDs
// Ring 7 - 8 LEDs
// One LED in the middle

int ring1 = 60;
int ring2 = 48;
int ring3 = 40;
int ring4 = 32;
int ring5 = 24;
int ring6 = 16;
int ring7 = 12;
int ring8 = 8;
int middle = 1;
int rings[] = {ring1, ring2, ring3, ring4, ring5, ring6, ring7, ring8, middle};
#define NUM_RINGS 9
#define NUM_LEDS 241
CRGB leds[NUM_LEDS];


void handleButton();

void setup() { 

  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(WIN_LEDS, OUTPUT);
  pinMode(TRY_AGAIN_LEDS, OUTPUT);
  digitalWrite(WIN_LEDS, LOW);
  digitalWrite(TRY_AGAIN_LEDS, LOW);
  digitalWrite(BUTTON_LED, HIGH);
  digitalWrite(BUZZER, LOW);
  attachInterrupt(digitalPinToInterrupt(BUTTON), handleButton, FALLING);

  FastLED.addLeds<WS2812B, LEDS_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(20);


  // Beep 4 times
  for (int i = 0; i < 5; i++) {
    tone(BUZZER, 480, 100);
    delay(200);
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    leds[i] = CRGB::Cyan;
    FastLED.show();
    delay(6);
  }

}





void loop() { 




}








void handleButton() {
    // Light each ring with a random color

  for (int i = 0; i < NUM_RINGS; i++) {
    for (int j = 0; j < rings[i]; j++) {
      leds[j] = CRGB(random(255), random(255), random(255));
    }
    FastLED.show();
  }

  
}
