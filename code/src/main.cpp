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
#include "helperFunctions.h"

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

// Game variables
int currentRing = 0;

// Easy - 1
// Medium - 2
// Hard - 3
int difficulty = 0;


void handleButton();
void selectDifficulty();

void setup() { 

  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(WIN_LEDS, OUTPUT);
  pinMode(TRY_AGAIN_LEDS, OUTPUT);
  digitalWrite(WIN_LEDS, LOW);
  digitalWrite(TRY_AGAIN_LEDS, LOW);
  digitalWrite(BUTTON_LED, LOW);
  digitalWrite(BUZZER, LOW);
  

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

  

  selectDifficulty();


}








void handleButton() {
    

}








void selectDifficulty() {
  // The user can switch between the difficulty levels by pressing the button
  // By holding the button for 3 seconds, the user can confirm the selection
  
  // Easy - 1
  // Medium - 2
  // Hard - 3
  
  bool confirmSelection = false;
  bool changeDifficulty = false;
  int difficulty = 1;
  unsigned long startMillis = millis(); // Store the start time
  const unsigned long timeout = 60000;  // 60 seconds timeout

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  fillRing(leds, CRGB::Green, rings, 0);
  FastLED.show();

  digitalWrite(BUTTON_LED, HIGH);
  
  while (!confirmSelection && (millis() - startMillis < timeout)) {  // Add timeout condition
    if (digitalRead(BUTTON) == LOW) {
      delay(50); // Debounce delay
      if (digitalRead(BUTTON) == LOW) { // Check again after debounce delay
        unsigned long lastMillis = millis();
        // Check if the button is pressed for 2 seconds
        while (digitalRead(BUTTON) == LOW) {
          if (millis() - lastMillis > 2000) {
            confirmSelection = true;
            tone(BUZZER, 480, 400);
            break;
          }
        }
        if (!confirmSelection) {
          tone(BUZZER, 800, 100);
          changeDifficulty = true;
        }
      }
    }     
    
    if (changeDifficulty) {
      difficulty++;
      if (difficulty > 3) {
        difficulty = 1;
      }
      
      switch (difficulty) {
        case 1:
          fill_solid(leds, NUM_LEDS, CRGB::Black);
          fillRing(leds, CRGB::Green, rings, 0);
          FastLED.show();
          break;
        case 2:
          fill_solid(leds, NUM_LEDS, CRGB::Black);
          fillRing(leds, CRGB::Yellow, rings, 1);
          FastLED.show();
          break;
        case 3:
          fill_solid(leds, NUM_LEDS, CRGB::Black);
          fillRing(leds, CRGB::Red, rings, 2);
          FastLED.show();
          break;
      }
      changeDifficulty = false;
    }
  }

  digitalWrite(BUTTON_LED, LOW);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  delay(500);
  
  if (confirmSelection) {  // Only blink if selection is confirmed
    // Blink the corresponding ring to indicate the selected difficulty
    for (int i = 0; i < 3; i++) {
      switch (difficulty) {
        case 1:
          fillRing(leds, CRGB::Green, rings, 0);
          break;
        case 2:
          fillRing(leds, CRGB::Yellow, rings, 1);
          break;
        case 3:
          fillRing(leds, CRGB::Red, rings, 2);
          break;
      }
      FastLED.show();
      delay(200);
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      delay(200);
    }
  } else {
    // If the selection is not confirmed run the function again
    selectDifficulty();
  }
}

  




  

