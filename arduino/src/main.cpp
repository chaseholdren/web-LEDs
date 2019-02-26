#include <Arduino.h>
#include <FastLED.h>
#include <ArduinoJson.h>
#include <SPI.h>

#define LED_PIN 8
#define NUM_LEDS 256
#define BRIGHTNESS 100
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
  Serial.println("OK");
}

// String inputString = ""; // a String to hold incoming data

void loop()
{
  String inputString = "";

  if (Serial.available())
  {
    inputString = String(Serial.readString());
    inputString.concat("/0");
  }

  if (inputString != "")
  {
    Serial.println("input string:");
    Serial.println(inputString);
    FastLED.clear();

    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    const size_t capacity = JSON_ARRAY_SIZE(3) + 3 * JSON_OBJECT_SIZE(3);
    DynamicJsonBuffer jsonBuffer(capacity);

    // Parse JSON object
    JsonArray& root = jsonBuffer.parseArray(inputString);
    const int arraySize = constrain(root.size(), 0, NUM_LEDS);
    Serial.println("array size:");
    Serial.println(arraySize);

    for (int i = 0; i < arraySize; i++)
    {
      JsonObject& ledObject = root[i];
      Serial.println("\nled object:");
      ledObject.printTo(Serial);
      Serial.println("\n");
      leds[i] = CRGB(ledObject["red"], ledObject["green"], ledObject["blue"]);
    }

    jsonBuffer.clear();

    // inputString = "";
    FastLED.show();
    Serial.println("OK");
  }
  // FastLED.delay(1000 / UPDATES_PER_SECOND);
}
