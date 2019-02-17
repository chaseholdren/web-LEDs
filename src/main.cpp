#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 8
#define NUM_LEDS 256
#define BRIGHTNESS 2
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
}

static uint8_t colorIndex = 0;

void loop()
{
  colorIndex = colorIndex + 1; /* motion speed */

  FastLED.clear();
  leds[colorIndex % NUM_LEDS] = CRGB::Red;

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}
