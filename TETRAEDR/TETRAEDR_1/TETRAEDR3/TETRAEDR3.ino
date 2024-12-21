// ===== НАСТРОЙКИ =====
#define EDGE_LEDS 24  // кол-во диодов на ребре куба
#define LED_DI 2      // пин подключения
#define BRIGHT 200    // яркость
#define THETIME 5

int bright = BRIGHT;

const int NUM_LEDS = (EDGE_LEDS * 6);
#include <FastLED.h>
CRGB leds[NUM_LEDS];
int head = 72;
int head_begin = 71;
int head_end = 96;
int comet_color = 0;
int shift_time = 100;
unsigned long prewmillis = 0;

int headr = 96;
int head_beginr = 96;
int head_endr = 71;
int comet_colorr = 0;
int shift_timer = 100;
unsigned long prewmillisr = 0;

unsigned long prewrefading = 0;
unsigned long prewfading = 0;
bool fadingdir = false;
bool flagfading = false;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, LED_DI, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(bright);
  leds[0].setRGB(100, 100, 255);
  FastLED.show();
  delay(1000);
  FastLED.clear();
  FastLED.show();
  randomSeed(analogRead(A0));
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - prewmillisr > shift_timer) {
    cometaReverced();
    prewmillisr = millis();
  }
  if (millis() - prewmillis > shift_time) {
    cometa();
    prewmillis = millis();
  }
 /* if (millis() - prewrefading > THETIME * 1000) {
    flagfading = true;
    fadingdir = true;
    prewrefading = millis();
  }
  if (flagfading) {
    fading();
  }*/
}

void cometa() {
  if (head - 11 == head_end) {
    head = random(0, 6) * EDGE_LEDS;
    head_end = head + EDGE_LEDS;
    head_begin = head - 1;
    shift_time = random(50, 90);
    comet_color = random(0, 255);
  }
  for (int i = 0; i < 11; i++) {
    if ((head - i < head_end) and (head - i > head_begin)) {
      leds[head - i].setHSV(comet_color, 255 - i * 15, 255 - i * 24);
    }
  }
  leds[head - 11].setRGB(0, 0, 0);
  head += 1;
  FastLED.show();
}

void cometaReverced() {
  if (headr + 11 == head_endr) {
    headr = random(1, 6) * EDGE_LEDS;
    head_endr = headr - EDGE_LEDS - 1;
    head_beginr = headr;
    shift_timer = random(50, 90);
    comet_colorr = random(0, 255);
  }
  for (int i = 0; i < 11; i++) {
    if ((headr + i > head_endr) and (headr + i < head_beginr)) {
      leds[headr + i].setHSV(comet_colorr, 255 - i * 15, 255 - i * 24);
    }
  }
  leds[headr + 11].setRGB(0, 0, 0);
  headr -= 1;
  FastLED.show();
}

void fading() {
  if (bright == 0) {
    fadingdir = false;
    delay(100);
  }
  if (fadingdir) {
    if (millis() - prewfading > 10) {
      bright -= 1;
      prewfading = millis();
    }
  } else {
    if (bright == BRIGHT) {
      flagfading = false;
    }
    if (millis() - prewfading > 10) {
      bright += 1;
      prewfading = millis();
    }
  }
  FastLED.setBrightness(bright);
}


/*
void simmetr() {
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i <= NUM_LEDS; i += EDGE_LEDS) {
      leds[i + j].setRGB(255 - j * 12, 0, 100);
      leds[i + 23 - j].setRGB(100, 0, 255 - j * 12);
    }
  }
}
*/