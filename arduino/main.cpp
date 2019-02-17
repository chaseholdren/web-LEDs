#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 8
#define NUM_LEDS 256
#define BRIGHTNESS 50
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType currentBlending;

void setup()
{
  Serial.println("setup1");
  delay(3000); // power-up safety delay
  Serial.println("setup2");
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
  Serial.println("setup3");
  Serial.begin(115200);
  Serial.println("setup4");
}

void loop()
{
  if (Serial.available() > 0)
  {
    int inputString = Serial.parseInt();
    Serial.flush();
    Serial.println(inputString);
    // colorIndex = colorIndex + 1; /* motion speed */
    if (inputString > 0 && inputString < NUM_LEDS)
    {
      FastLED.clear();
      leds[inputString % NUM_LEDS] = CRGB::Red;

      FastLED.show();
    }
  }
  // delay(1000);
}
