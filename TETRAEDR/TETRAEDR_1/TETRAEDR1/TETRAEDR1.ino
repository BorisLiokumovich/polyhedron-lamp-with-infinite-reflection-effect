// ===== НАСТРОЙКИ =====
#define EDGE_LEDS 24    // кол-во диодов на ребре куба
#define LED_DI 2        // пин подключения
#define BRIGHT 255      // яркость

const int NUM_LEDS = (EDGE_LEDS * 6);
#include <FastLED.h>
CRGB leds[NUM_LEDS];
int head = 72;
int head_begin = 71;
int head_end = 96;
bool check = false;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, LED_DI, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHT);
  FastLED.clear();

}

void loop() {
  // put your main code here, to run repeatedly:
  cometa();
}

void cometa() {
  if (head - 11 == head_end) {
    head = random(1, 6) * EDGE_LEDS;
    head_end = head + 24;
    head_begin = head - 1;
  }
  for (int i = 0; i < 11; i++) {
    if ((head - i < head_end) and (head - i > head_begin ) ) {
      leds[head - i].setRGB(200 - i * 20, 200 - i * 20, 200 - i * 20);
    }
  }
  leds[head - 11].setRGB(0, 0, 0);
  head += 1;
  FastLED.show();
  delay(75);
}

void simmetr() {
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i <= NUM_LEDS; i += EDGE_LEDS) {
      leds[i + j].setRGB(255 - j * 12, 0, 100);

      leds[i + 23 - j].setRGB(100, 0, 255 - j * 12);
    }
  }
}
