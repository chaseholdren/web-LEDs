#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 8
#define NUM_LEDS 256
#define BRIGHTNESS 100
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType currentBlending;

void setup()
{
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalette = LavaColors_p;
  currentBlending = LINEARBLEND;
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB
  }

  // static uint8_t colorIndex = 255;
  // leds[colorIndex % NUM_LEDS] = CRGB::Red;

  // FastLED.show();
  Serial.begin(115200);
  Serial.println("OK");
}

void loop()
{
  if (Serial.available() > 0)
  {

    int inputString = Serial.parseInt();
    if (inputString > 0 && inputString < NUM_LEDS)
    {

      FastLED.clear();
      leds[inputString % NUM_LEDS] = CRGB::Red;

      FastLED.show();
    }

    Serial.println(inputString);
    Serial.println("OK");
  }
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}
