#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 8
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 32
#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)
#define BRIGHTNESS 255
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

unsigned long lastShowTime;

const int colorModeButtonPin = 2;
int colorModeButtonState = 0;
int lastColorModeButtonState = 0;

const int speedDownButtonPin = 3;
int speedDownButtonState = 0;
int lastSpeedDownButtonState = 0;

const int speedUpButtonPin = 4;
int speedUpButtonState = 0;
int lastSpeedUpButtonState = 0;

void setup()
{
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(colorModeButtonPin, INPUT);
  pinMode(speedDownButtonPin, INPUT);
  pinMode(speedUpButtonPin, INPUT);

  Serial.begin(115200);
  delay(2000); while (!Serial);
}

int UPDATES_PER_SECOND = 2;

// row starts at 192;
const int startingLed = 193;
const int endingLed = 222;
int currentLed = startingLed;
bool goingUp = true;
bool threeColorMode = true;

// "boustrophedon", meaning "as the ox plows."
uint16_t xyFromIndex(uint16_t index)
{
  int x = floor(index / MATRIX_HEIGHT);
  int y = index % MATRIX_HEIGHT;

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

void handleButtonPresses()
{
  lastColorModeButtonState = colorModeButtonState;
  lastSpeedDownButtonState = speedDownButtonState;
  lastSpeedUpButtonState = speedUpButtonState;

  colorModeButtonState = digitalRead(colorModeButtonPin);
  speedDownButtonState = digitalRead(speedDownButtonPin);
  speedUpButtonState = digitalRead(speedUpButtonPin);

  if(colorModeButtonState == HIGH && lastColorModeButtonState == LOW) {
    threeColorMode = !threeColorMode;
  }

  if(speedUpButtonState == HIGH && lastSpeedUpButtonState == LOW) {
    UPDATES_PER_SECOND = constrain(UPDATES_PER_SECOND * 2, 1, 5000);
  } else {
    if (speedDownButtonState == HIGH && lastSpeedDownButtonState == LOW)
    {
      UPDATES_PER_SECOND = constrain(UPDATES_PER_SECOND / 2, 1, 5000);
    }
  }


}


void loop()
{
  handleButtonPresses();

  const long timeSinceLastShow = millis() - lastShowTime;
  const int showTimeInterval = 1000 / UPDATES_PER_SECOND;
  if (timeSinceLastShow >= showTimeInterval)
  {
    Serial.println(UPDATES_PER_SECOND);
    Serial.println(showTimeInterval);
    lastShowTime = millis();

    if (goingUp)
    {
      currentLed++;
    }
    else
    {
      currentLed--;
    }

    FastLED.clear();

    if (threeColorMode)
    {
      leds[xyFromIndex(currentLed - 2)] = CRGB::Black;
      leds[xyFromIndex(currentLed - 1)] = CRGB::DarkBlue;
      leds[xyFromIndex(currentLed)] = CRGB::DarkGreen;
      leds[xyFromIndex(currentLed + 1)] = CRGB::DarkRed;
      leds[xyFromIndex(currentLed + 2)] = CRGB::Black;
    }
    else
    {
      leds[xyFromIndex(currentLed - 1)] = CRGB::Black;
      leds[xyFromIndex(currentLed)] = CRGB::White;
      leds[xyFromIndex(currentLed + 1)] = CRGB::Black;
    }
    
    FastLED.show();

    if (currentLed == endingLed)
      goingUp = false;
    if (currentLed == startingLed)
      goingUp = true;
  }
}
