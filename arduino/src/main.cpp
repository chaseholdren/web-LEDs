#include <Arduino.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <SPI.h>

#define LED_PIN 8
#define NUM_LEDS 256
#define BRIGHTNESS 25
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

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
  Serial.setTimeout(5000);
  Serial.println("OK");
}

// String inputString = ""; // a String to hold incoming data

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
          leds[i] = CRGB(colorData["red"], colorData["green"], colorData["blue"]);
        }
      }
      FastLED.show();

      Serial.println("OK");
    }
  }
  // FastLED.delay(1000 / UPDATES_PER_SECOND);
}
