#include <Arduino.h>
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define DECODE_NEC 1
#include <FastLED.h>
#include <IRremote.h>

#define LED_PIN 8
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 32
#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)
#define BRIGHTNESS 255
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

unsigned long lastShowTime;

//Create a receiver object to listen on pin

IRrecv myReceiver(11);

//+=============================================================================
// Display IR code
//
void ircode(decode_results *results)
{
    // Panasonic has an Address
    if (results->decode_type == PANASONIC)
    {
        Serial.print(results->address, HEX);
        Serial.print(":");
    }

    // Print Code
    Serial.print(results->value, HEX);
}

//+=============================================================================
// Display encoding type
//
void encoding(decode_results *results)
{
    switch (results->decode_type)
    {
    default:
    case UNKNOWN:
        Serial.print("UNKNOWN");
        break;
    case NEC:
        Serial.print("NEC");
        break;
    case SONY:
        Serial.print("SONY");
        break;
    case RC5:
        Serial.print("RC5");
        break;
    case RC6:
        Serial.print("RC6");
        break;
    case DISH:
        Serial.print("DISH");
        break;
    case SHARP:
        Serial.print("SHARP");
        break;
    case JVC:
        Serial.print("JVC");
        break;
    case SANYO:
        Serial.print("SANYO");
        break;
    case MITSUBISHI:
        Serial.print("MITSUBISHI");
        break;
    case SAMSUNG:
        Serial.print("SAMSUNG");
        break;
    case LG:
        Serial.print("LG");
        break;
    case WHYNTER:
        Serial.print("WHYNTER");
        break;
    case AIWA_RC_T501:
        Serial.print("AIWA_RC_T501");
        break;
    case PANASONIC:
        Serial.print("PANASONIC");
        break;
    case DENON:
        Serial.print("Denon");
        break;
    }
}

//+=============================================================================
// Dump out the decode_results structure.
//
void dumpInfo(decode_results *results)
{
    // Check if the buffer overflowed
    if (results->overflow)
    {
        Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWBUF");
        return;
    }

    if (results->decode_type != NEC || results->value == 4294967295)
        return;

    // Show Encoding standard
    Serial.print("Encoding  : ");
    encoding(results);
    Serial.println("");

    // Show Code & length
    Serial.print("Code      : ");
    ircode(results);
    Serial.println("");
    Serial.println("");
}

void setup()
{
    delay(3000); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);

    Serial.begin(115200);
    delay(2000);
    while (!Serial)
        ;
    myReceiver.enableIRIn();
    Serial.println("Ready to receive IR signals");
    lastShowTime = millis();
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

void handleRemotePresses(decode_results results)
{
    if (results.decode_type != NEC || results.value == 4294967295)
        return;

    switch (results.value)
    {
    default:
        break;
    case 16724175:
        threeColorMode = !threeColorMode;
        break;
    case 16718055:
        UPDATES_PER_SECOND = constrain(UPDATES_PER_SECOND / 2, 1, 5000);
        break;
    case 16743045:
        UPDATES_PER_SECOND = constrain(UPDATES_PER_SECOND * 2, 1, 5000);
        break;
    }
}

void loop()
{
    decode_results results; // Somewhere to store the results

    if (myReceiver.decode(&results))
    {                       // Grab an IR code
        dumpInfo(&results); // Output the results
        Serial.println(results.value, HEX);
        Serial.println(results.value);
        handleRemotePresses(results);

        myReceiver.enableIRIn();
    }

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
