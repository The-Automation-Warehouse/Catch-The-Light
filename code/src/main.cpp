/*
 * Catch-The-Light
 * This is a crude arcade game inspired by "Speed Demon".
  * It consists of a ring of LEDs through which a light travels.
  * In each round, the light travels through the ring and the player has to press a button at the right time to stop the light.
  * The difficulty of the game changes the number of target LEDs that light up in each ring.
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
#define WIN_LEDS 6
#define TRY_AGAIN_LEDS 5

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

bool doInterrupt = false;

// Game variables
int currentRing = 0;
int currentLED = 0;
int stopLED = 0;
int targetLEDs[5] = {0, 0, 0, 0, 0};
int totalLEDs = 0;
bool nextRound = false;
bool gameWon = false;
bool gameOver = false;
// Easy - 1
// Medium - 2
// Hard - 3
int difficulty = 0;


void handleButton();
void selectDifficulty();
void playGame();

void setup() { 

  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(WIN_LEDS, OUTPUT);
  pinMode(TRY_AGAIN_LEDS, OUTPUT);
  digitalWrite(WIN_LEDS, HIGH);
  digitalWrite(TRY_AGAIN_LEDS, HIGH);
  digitalWrite(BUTTON_LED, LOW);
  digitalWrite(BUZZER, LOW);

  randomSeed(analogRead(A0));

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

  digitalWrite(WIN_LEDS, LOW);
  digitalWrite(TRY_AGAIN_LEDS, LOW);

}





void loop() { 

  

  selectDifficulty();
  Serial.println("Difficulty selected: ");
  Serial.println(difficulty);
  playGame();


}






void handleButton() {

  if (doInterrupt) {
    stopLED = currentLED;
    // Check if the button is pressed at the right time
    if (stopLED == targetLEDs[0] || stopLED == targetLEDs[1] || stopLED == targetLEDs[2] || stopLED == targetLEDs[3] || stopLED == targetLEDs[4]) {
      // If the button is pressed at the right time, the light progresses to the next ring
      currentRing++;
      if (currentRing == NUM_RINGS) {
        gameOver = true;
        gameWon = true;
        doInterrupt = false;
      } else {
        nextRound = true;
        doInterrupt = false;
      }
    } else {
      // If the button is pressed at the wrong time, the game is over
      gameOver = true;
      nextRound = true;
      doInterrupt = false;
    }
    Serial.print("Stopped at: ");
    Serial.println(stopLED);
  }

}








void selectDifficulty() {
  // The user can switch between the difficulty levels by pressing the button
  // The difficulty changes how many LEDs light up in each ring
  // By holding the button for 3 seconds, the user can confirm the selection
  
  // Easy - 1
  // Medium - 2
  // Hard - 3
  
  bool confirmSelection = false;
  bool changeDifficulty = false;
  difficulty = 1;
  unsigned long startMillis = millis(); // Store the start time
  const unsigned long timeout = 60000;  // 60 seconds timeout

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  fillRing(leds, CRGB::Green, rings, 0);
  fillRing(leds, CRGB::Green, rings, 1);
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
          fillRing(leds, CRGB::Green, rings, 1);
          FastLED.show();
          break;
        case 2:
          fill_solid(leds, NUM_LEDS, CRGB::Black);
          fillRing(leds, CRGB::Yellow, rings, 2);
          fillRing(leds, CRGB::Yellow, rings, 3);
          FastLED.show();
          break;
        case 3:
          fill_solid(leds, NUM_LEDS, CRGB::Black);
          fillRing(leds, CRGB::Red, rings, 4);
          fillRing(leds, CRGB::Red, rings, 5);
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
          fillRing(leds, CRGB::Green, rings, 1);
          break;
        case 2:
          fillRing(leds, CRGB::Yellow, rings, 2);
          fillRing(leds, CRGB::Yellow, rings, 3);
          break;
        case 3:
          fillRing(leds, CRGB::Red, rings, 4);
          fillRing(leds, CRGB::Red, rings, 5);
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

  



















  

void playGame() {
  while (!gameOver)
  {
    Serial.print("Current ring: ");
    Serial.println(currentRing);
    totalLEDs = 0;
    currentLED = 0;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    // Pick a random LED to light up as the target depending on the difficulty
    // Easy - 5 LED
    // Medium - 3 LEDs
    // Hard - 1 LEDs

resetTarget:
    switch (difficulty) {
      case 1:
      {
        Serial.println("Easy");
        for (int j = 0; j < currentRing; j++) {
          totalLEDs += rings[j];
        }
        targetLEDs[2] = random(totalLEDs, totalLEDs + rings[currentRing]);
        targetLEDs[3] = targetLEDs[2] + 1;
        targetLEDs[4] = targetLEDs[2] + 2;
        targetLEDs[1] = targetLEDs[2] - 1;
        targetLEDs[0] = targetLEDs[2] - 2;
      }
        break;
      case 2:
      {
        Serial.println("Medium");
        for (int j = 0; j < currentRing; j++) {
          totalLEDs += rings[j];
        }
        targetLEDs[1] = random(totalLEDs, totalLEDs + rings[currentRing]);
        targetLEDs[2] = targetLEDs[1] + 1;
        targetLEDs[0] = targetLEDs[1] - 1;
      }
        break; 
      case 3:
      {
        Serial.println("Hard");
        targetLEDs[0] = targetLEDs[2] - 2;
        for (int j = 0; j < currentRing; j++) {
          totalLEDs += rings[j];
        }
          targetLEDs[0] = random(totalLEDs, totalLEDs + rings[currentRing]);
        }
          break;
      }

      // Check if the target LEDs are not out of bounds
      for (int j = 0; j < 5; j++) {
        if (targetLEDs[j] < totalLEDs || targetLEDs[j] > totalLEDs + rings[currentRing]) {
          for (int j = 0; j < 5; j++) {
            targetLEDs[j] = 0;
          }
          totalLEDs = 0;
          Serial.println("Target out of bounds");
          goto resetTarget;
        }
      }

      Serial.println("Target LEDs: ");
      for (int j = 0; j < 5; j++) {
        Serial.print(targetLEDs[j]);
        Serial.print(" ");
      }

      // Light up the target LEDs
      for (int j = 0; j < 5; j++) {
        if (targetLEDs[j] != 0) {
          leds[targetLEDs[j]] = CRGB::Yellow;
        }
      }
      FastLED.show();
      delay(300);
      attachInterrupt(digitalPinToInterrupt(BUTTON), handleButton, FALLING);
      delay(200);
      doInterrupt = true;
      while (!nextRound) {
        // Play the actual game
        fadeToBlackBy(leds, NUM_LEDS, 60);
        
        // Light up the target LEDs
        for (int j = 0; j < 5; j++) {
          if (targetLEDs[j] != 0) {
            leds[targetLEDs[j]] = CRGB::Yellow;
          }
        }
        //Draw the moving light
        leds[currentLED] = CRGB::Green;
        FastLED.show();
        delay(16);
        currentLED++;

        if (currentLED == totalLEDs + rings[currentRing]) {
          currentLED = totalLEDs;
        }
      }
      doInterrupt = false;
      detachInterrupt(digitalPinToInterrupt(BUTTON));
      nextRound = false;
  }
  if (gameWon) {
    // Light up the win LEDs
    digitalWrite(WIN_LEDS, HIGH);
    for (int i = 0; i < 3; i++) {
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      FastLED.show();
      delay(500);
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      delay(500);
    }
    digitalWrite(WIN_LEDS, LOW);
  } else {
    // Light up the try again LEDs
    digitalWrite(TRY_AGAIN_LEDS, HIGH);
    for (int i = 0; i < 3; i++) {
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      FastLED.show();
      delay(500);
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      delay(500);
    }
    digitalWrite(TRY_AGAIN_LEDS, LOW);
  }

  // Reset the game variables
  currentRing = 0;
  currentLED = 0;
  stopLED = 0;
  for (int j = 0; j < 5; j++) {
    targetLEDs[j] = 0;
  }
  nextRound = false;
  gameWon = false;
  gameOver = false;
  doInterrupt = false;
  
  // Play the loading animation
  for (int i = 0; i < NUM_LEDS; i++) {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    leds[i] = CRGB::Cyan;
    FastLED.show();
    delay(6);
  }
}



