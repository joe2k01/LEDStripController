#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "config.h"
#include <FastLED.h>
#define LEDs 30
#define DIN 2

CRGB leds[LEDs];
BlynkTimer timer;

int s = 0;
int rainbowTimer;

void rainbowHandler();

void setup()
{
  FastLED.addLeds<NEOPIXEL, DIN>(leds, LEDs);

  Blynk.begin(TOKEN, APNAME, PWD);

  rainbowTimer = timer.setInterval(1L, rainbowHandler);
  timer.disable(rainbowTimer);

  Blynk.virtualWrite(V1, LOW);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void rainbowHandler() {
   fill_rainbow(&leds[0], LEDs, s);
   FastLED.show();
   FastLED.delay(2);
   s++;
   if (s == 224)
      s = 0;
}

BLYNK_WRITE(V1) {
  if (param.asInt() == HIGH) {
    CRGB colour = leds[0];
      CRGB black = CRGB::Black;
      if (colour != black) {
        for (int i = 255; i > 0; i--) {
          FastLED.setBrightness(i);
          for (int j = 0; j < LEDs; j++) {
            colour = leds[j];
            leds[j] = colour;
          }
          FastLED.show();
          FastLED.delay(2);
        }
      }
    FastLED.show();
    FastLED.setBrightness(255);
    timer.enable(rainbowTimer);
  } else {
    timer.disable(rainbowTimer);
  }
}

BLYNK_WRITE(V2) {
  Blynk.virtualWrite(V1, LOW);
  timer.disable(rainbowTimer);
  CRGB colour = leds[0];
  CRGB black = CRGB::Black;
  if (colour != black) {
    for (int i = 255; i > 0; i--) {
      FastLED.setBrightness(i);
      for (int j = 0; j < LEDs; j++) {
          colour = leds[j];
          leds[j] = colour;
        }
      FastLED.show();
      FastLED.delay(2);
      FastLED.show();
      FastLED.delay(2);
      }
    }
    for (int i = 0; i < 256; i++) {
      FastLED.setBrightness(i);
      fill_solid(&leds[0], LEDs, CRGB(param[0].asInt(), param[1].asInt(), param[2].asInt()));
      FastLED.show();
      FastLED.delay(2);
    }
  }
