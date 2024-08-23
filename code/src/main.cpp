#include <Arduino.h>
#include <FastLED.h>

#define BUTTON 2
#define LEDS_PIN 3
#define BUTTON_LED 4
#define BUZZER 9
#define WIN_LEDS 5
#define TRY_AGAIN_LEDS 6


#define NUM_LEDS 300
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
  for (int i = 0; i < 4; i++) {
    tone(BUZZER, 1000, 100);
    delay(200);
  }

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








void handleButton() {
  digitalWrite(BUTTON_LED, !digitalRead(BUTTON_LED));
  tone(BUZZER, 800, 400);
}
