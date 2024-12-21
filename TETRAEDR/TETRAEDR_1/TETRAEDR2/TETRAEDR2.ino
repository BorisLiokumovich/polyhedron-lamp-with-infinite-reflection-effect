// ===== НАСТРОЙКИ =====
#define EDGE_LEDS 24  // кол-во диодов на ребре куба
#define LED_DI 2      // пин подключения
#define BRIGHT 200    // яркость

const int NUM_LEDS = (EDGE_LEDS * 6);
#include <FastLED.h>
CRGB leds[NUM_LEDS];
//int head = 72;
//int head_begin = 71;
//int head_end = 96;
//int comet_color = 0;
//int shift_time = 100;

class COMETA {
public:
  void update() {
    for (int i = 0; i < 11; i++) {
      if ((head - i < head_end) and (head - i > head_begin)) {
        leds[head - i].setHSV(comet_color, 255 - i * 15, 255 - i * 25);
      }
    }
    leds[head - 11].setRGB(0, 0, 0);
    head += 1;
    FastLED.show();
    if (head - 11 == head_end) {
      recreate();
    }
  }
  void recreate() {
    head = random(0, 6) * EDGE_LEDS;
    head_end = head + EDGE_LEDS;
    head_begin = head - 1;
    shift_time = random(35, 90);
    comet_color = random(0, 255);
  }
private:
  int head;
  int head_end;
  int head_begin;
  int shift_time;
  int comet_color;
};

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, LED_DI, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHT);
 leds[0].setRGB(100, 100, 255);
  FastLED.show();
  delay(1000);
  FastLED.clear();
 FastLED.show();
  randomSeed(analogRead(A0));
}

COMETA comet;

void loop() {
  // put your main code here, to run repeatedly:
  comet.update();
}

/*
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
  delay(shift_time);
}
*/


void
simmetr() {
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i <= NUM_LEDS; i += EDGE_LEDS) {
      leds[i + j].setRGB(255 - j * 12, 0, 100);

      leds[i + 23 - j].setRGB(100, 0, 255 - j * 12);
    }
  }
}
