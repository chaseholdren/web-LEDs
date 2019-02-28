#include <Arduino.h>
#include <FastLED.h>
#include <math.h>
#include <ArduinoJson.h>
#include <SPI.h>

#define LED_PIN 8
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 32
#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)
#define BRIGHTNESS 10
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// "boustrophedon", meaning "as the ox plows."
uint16_t XY(uint8_t x, uint8_t y)
{
  uint16_t i;

  if (y & 0x01)
  {
    // Odd rows run backwards
    uint8_t reverseX = (MATRIX_WIDTH - 1) - x;
    i = (y * MATRIX_WIDTH) + reverseX;
  }
  else
  {
    // Even rows run forwards
    i = (y * MATRIX_WIDTH) + x;
  }

  return i;
}

uint16_t xyFromIndex(uint16_t index)
{
  int x = floor(index / MATRIX_HEIGHT);
  int y = index % MATRIX_HEIGHT;
  return XY(x, y);
}

void setup()
{
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  leds[0] = CRGB::White;

  FastLED.show();
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.begin(115200);
  Serial.println("OK");
}

void loop()
{

  if (Serial.available())
  {
    const int colorCount = Serial.parseInt();

    if (colorCount != 0)
    {
      for (int i = 0; i < colorCount; i++)
      {
        // Allocate JsonBuffer
        // Use arduinojson.org/assistant to compute the capacity.
        const int capacity = JSON_OBJECT_SIZE(3) + 30;
        StaticJsonBuffer<capacity> jsonBuffer;

        // Parse JSON object
        JsonObject &colorData = jsonBuffer.parseObject(Serial);
        if (colorData.success())
        {
          leds[xyFromIndex(i)] = CRGB(colorData["red"], colorData["green"], colorData["blue"]);
        }
      }
      FastLED.show();

      Serial.println("OK");
    }
  }
}
