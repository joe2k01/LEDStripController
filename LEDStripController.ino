#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "config.h"
#include <FastLED.h>
#define LEDs 31
#define DIN 2

CRGB leds[LEDs];
BlynkTimer timer;

int s = 0;
bool increase = true;
int rainbowTimer;

void rainbowHandler();

void setup()
{
  FastLED.addLeds<NEOPIXEL, DIN>(leds, LEDs);
  fill_solid(&leds[0], LEDs, CRGB::Red);
  FastLED.show();

  Blynk.begin(TOKEN, APNAME, PWD);

  rainbowTimer = timer.setInterval(1L, rainbowHandler);
  timer.disable(rainbowTimer);

  fill_solid(&leds[0], LEDs, CRGB::Black);
  FastLED.show();
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
   if (increase)
     s++;
   else
     s--;
   if (s == 224)
     increase = false;
   else if (s == 0)
     increase = true;
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
      }
    }
    for (int i = 0; i < 256; i++) {
      FastLED.setBrightness(i);
      fill_solid(&leds[0], LEDs, CRGB(param[0].asInt(), param[1].asInt(), param[2].asInt()));
      FastLED.show();
      FastLED.delay(2);
    }
  }
