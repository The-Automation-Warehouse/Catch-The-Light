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

int ring0 = 60;
int ring1 = 48;
int ring2 = 40;
int ring3 = 32;
int ring4 = 24;
int ring5 = 16;
int ring6 = 12;
int ring7 = 8;
int middle = 1;
int rings[] = {ring0, ring1, ring2, ring3, ring4, ring5, ring6, ring7, middle};
#define NUM_RINGS 7
#define NUM_LEDS 241
CRGB leds[NUM_LEDS];

bool doInterrupt = false;

// Game variables
int currentRing = 0;
int currentLED = 0;
int stopLED = 0;
int targetLEDs[5] = {0, 0, 0, 0, 0};
int oldTargetLEDs[80] = {0}; // Stores the previous target LEDs so that they can be lit up again in the next rounds
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
void fillRing(CRGB leds[], CRGB color, int ringSizes[], int ringIndex);
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
    // Check if the button is pressed at the right time (the stopLED is any of the targetLEDs)
    if (stopLED == targetLEDs[0] || stopLED == targetLEDs[1] || stopLED == targetLEDs[2] || stopLED == targetLEDs[3] || stopLED == targetLEDs[4]) {
      // If the button is pressed at the right time, the light progresses to the next ring
      if (currentRing == NUM_RINGS) {
        Serial.println("Game won!");
        gameOver = true;
        gameWon = true;
        doInterrupt = false;
      } else {
        currentRing++;
        nextRound = true;
        doInterrupt = false;
      }
    } else {
      // If the button is pressed at the wrong time, the game is over
      Serial.println("Game over!");
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

  






void fillRing(CRGB leds[], CRGB color, int ringSizes[], int ringIndex) {
    int tottalLeds = 0;
    for (int i = 0; i < ringIndex+1; i++) {
        tottalLeds += ringSizes[i];
    }
    
  for (int i = tottalLeds - ringSizes[ringIndex]; i < tottalLeds; i++) {
    leds[i] = color;
  }
}












  

void playGame() {
  while (!gameOver)
  {
    if (gameOver) {
      break;
    }
    Serial.print("Current ring: ");
    Serial.println(currentRing);
    totalLEDs = 0;
    currentLED = 0;
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    // Pick a random LED to light up as the target depending on the difficulty
    // Easy - 5 LED
    // Medium - 4 LEDs
    // Hard - 3 LEDs

resetTarget:

    for (int j = 0; j < currentRing; j++) {
      totalLEDs += rings[j];
    }
    targetLEDs[0] = random(totalLEDs, totalLEDs + rings[currentRing]);

    switch (difficulty) {
      case 1:
      {
        Serial.println("Easy");
        targetLEDs[1] = targetLEDs[0] + 1;
        targetLEDs[2] = targetLEDs[0] + 2;
        targetLEDs[3] = targetLEDs[0] - 1;
        targetLEDs[4] = targetLEDs[0] - 2;
      }
        break;
      case 2:
      {
        Serial.println("Medium");
        targetLEDs[1] = targetLEDs[0] + 1;
        targetLEDs[2] = targetLEDs[0] + 2;
        targetLEDs[3] = targetLEDs[0] - 1;
      }
        break; 
      case 3:
      {
        Serial.println("Hard");
        targetLEDs[1] = targetLEDs[0] + 1;
        targetLEDs[2] = targetLEDs[0] - 1;
      }
        break;
      }

      // Print the target LEDs
      Serial.print("Target LEDs: ");
      for (int j = 0; j < 5; j++) {
        Serial.print(targetLEDs[j]);
        Serial.print(" ");
      }
      Serial.println();

      // Print the old target LEDs
      Serial.print("Old target LEDs: ");
      for (int j = 0; j < 50; j++) {
        Serial.print(oldTargetLEDs[j]);
        Serial.print(" ");
      }
      Serial.println();
      // Check if the target LEDs are not out of bounds
      for (int j = 0; j < 5; j++) {
        if (targetLEDs[j] < totalLEDs || targetLEDs[j] + 1 > totalLEDs + rings[currentRing]) {
          for (int j = 0; j < 5; j++) {
            targetLEDs[j] = 0;
          }
          totalLEDs = 0;
          Serial.println("Target out of bounds");
          randomSeed(analogRead(A0));
          goto resetTarget;
        }
      }


      // Light up the target LEDs
      for (int j = 0; j < 5; j++) {
        if (targetLEDs[j] != 0) {
          leds[targetLEDs[j]] = CHSV(64, 255, 128);
        }
      }
      FastLED.show();
      delay(300);
      attachInterrupt(digitalPinToInterrupt(BUTTON), handleButton, FALLING);
      delay(200);
      doInterrupt = true;
      while (!nextRound && !gameOver) {
        // Play the actual game

        // Adjust the fade speed depending on the current ring but cap it at 160
        fadeToBlackBy(leds, NUM_LEDS, min(160, (currentRing + 1) * 25));
        
        // Light up the target LEDs with 50% brightness yellow
        for (int j = 0; j < 5; j++) {
          if (targetLEDs[j] != 0) {
            leds[targetLEDs[j]] = CHSV(64, 255, 128);
          }
        }
        // Light up the old target LEDs with 50% brightness red
        for (unsigned int j = 0; j < sizeof(oldTargetLEDs) / sizeof(oldTargetLEDs[0]); j++) {
            if (oldTargetLEDs[j] != 0) {
                leds[oldTargetLEDs[j]] = CHSV(0, 255, 128);
            }
        }
        // Draw the moving light
        // Change the color of the light depending on the ring (if its in the current ring or not)
        if (currentLED < totalLEDs) {
          leds[currentLED] = CRGB::Green;
          delay(10);
        } else {
          leds[currentLED] = CRGB::Blue;
          delay(26);
        }
        FastLED.show();
        
        currentLED++;

        if (currentLED == totalLEDs + rings[currentRing]) {
          currentLED = totalLEDs;
        }
      }
      doInterrupt = false;
      detachInterrupt(digitalPinToInterrupt(BUTTON));
      nextRound = false;

      // Add the target LEDs to the old target LEDs so that they can be lit up again in the next rounds
      // The old target LEDs are stored in a array where each ring is given 10 spaces
      for (int j = 0; j < 5; j++) {
        if (targetLEDs[j] != 0) {
          oldTargetLEDs[currentRing * 10 + j] = targetLEDs[j];
        }
      }
  }
  Serial.println("Game over, playing animation");
  if (gameWon) {
    // Light up the win LEDs and play a winning sound and animation
    // The animation lights up every second LED in the ring and alternates odd and even LEDs
    for (int i = 0; i < 10; i++) {
      tone(BUZZER, 1200, 100);
      digitalWrite(WIN_LEDS, HIGH);
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      for (int j = 0; j < NUM_LEDS/2; j++) {
        leds[j*2] = CRGB::Green;
      }
      FastLED.show();
      tone(BUZZER, 1200, 100);
      delay(100);
      tone(BUZZER, 1200, 100);
      delay(100);
      tone(BUZZER, 1200, 100);
      digitalWrite(WIN_LEDS, LOW);
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      for (int j = 0; j < NUM_LEDS/2; j++) {
        leds[j*2+1] = CRGB::Green;
      }
      FastLED.show();
      delay(200);
    }


  } else {
    // Blink the try again LEDs and play and angry beeping sound
    for (int i = 0; i < 3; i++) {
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      digitalWrite(TRY_AGAIN_LEDS, HIGH);
      FastLED.show();
      tone(BUZZER, 100, 200);
      delay(200);
      digitalWrite(TRY_AGAIN_LEDS, LOW);
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      delay(200);
    }
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



