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

unsigned long prewcreateglowig = 0;
int glow_color[NUM_LEDS];
int glow_bright[NUM_LEDS];
bool create_glow = true;
int glow_color_seed = 0;
int glow_color_shift = 20;
int regim = 3;

unsigned long prewsimm = 0;
int simm_leds_color[NUM_LEDS / 2];
int simm_leds_counter = 0;
int simm_leds_shift = 0;
int simm_leds_time = 50;

void setup() {
  // put your setup code here, to run once:
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 6000);
  FastLED.addLeds<WS2812, LED_DI, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(bright);
  leds[0].setRGB(100, 100, 255);
  FastLED.show();
  delay(1000);
  FastLED.clear();
  FastLED.show();
  randomSeed(analogRead(A0));
  for (int i = 0; i < NUM_LEDS; i++) {
    glow_bright[i] = 0;
  }
  for (int i = 0; i < NUM_LEDS / 2; i++) {
    simm_leds_color[i] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (regim) {
    case 0:
      if (millis() - prewmillisr > shift_timer) {
        cometaReverced();
        prewmillisr = millis();
      }
      if (millis() - prewmillis > shift_time) {
        cometa();
        prewmillis = millis();
      }
      break;
    case 1:
      if (millis() - prewcreateglowig > 50) {
        glowing();
        prewcreateglowig = millis();
        create_glow = true;
      }
      break;
    case 2:
      if (millis() - prewcreateglowig > 50) {
        glowing();
        prewcreateglowig = millis();
        create_glow = true;
      }
      break;
    case 3:
      if (millis() - prewsimm > simm_leds_time) {
        simmetr();
        prewsimm = millis();
      }
      break;
  }
  if (millis() - prewrefading > THETIME * 1000) {
    flagfading = true;
    fadingdir = true;
    prewrefading = millis();
  }
  if (flagfading) {
    fading();
  }
}

void simmetr() {
  for (int i = 0; i < EDGE_LEDS / 2; i++) {
    simm_leds_color[i] = simm_leds_shift * i + simm_leds_counter;
    simm_leds_counter -= 1;
  }
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < EDGE_LEDS / 2; j++) {
      leds[i * EDGE_LEDS + j].setHSV(simm_leds_color[j], 255, 255);
      leds[(i + 1) * EDGE_LEDS - j - 1].setHSV(simm_leds_color[j], 255, 255);
    }
  }
  FastLED.show();
}


void glowing() {
  int randglow = random(NUM_LEDS);
  if (create_glow) {
    glow_bright[randglow] = 255;
    if (glow_color_seed == 0) {
      glow_color[randglow] = random(255);
    } else {
      glow_color[randglow] = glow_color_seed + random(-glow_color_shift, glow_color_shift);
    }
    leds[randglow].setHSV(glow_color[randglow], 255, glow_bright[randglow]);
    create_glow = false;
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    if (glow_bright[i] > 0) {
      glow_bright[i] -= 10;
      if (glow_bright[i] < 0) {
        glow_bright[i] = 0;
      }
      leds[i].setHSV(glow_color[i], 255, glow_bright[i]);
    }
  }
  FastLED.show();
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
    if ((head - i < head_end) and (head - i > head_begin) and (headr - i >= 0)) {
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
    if ((headr + i > head_endr) and (headr + i < head_beginr) and (headr + i >= 0)) {
      leds[headr + i].setHSV(comet_colorr, 255 - i * 15, 255 - i * 24);
    }
  }
  leds[headr + 11].setRGB(0, 0, 0);
  headr -= 1;
  FastLED.show();
}

void fading() {
  if (bright <= 0) {
    fadingdir = false;
    // regim = random(4);
    switch (regim) {
      case 0:
        headr = random(1, 6) * EDGE_LEDS;
        head_endr = headr - EDGE_LEDS - 1;
        head_beginr = headr;
        shift_timer = random(50, 90);
        comet_colorr = random(0, 255);

        head = random(0, 6) * EDGE_LEDS;
        head_end = head + EDGE_LEDS;
        head_begin = head - 1;
        shift_time = random(50, 90);
        comet_color = random(0, 255);
        break;
      case 1:
        glow_color_seed = 0;
        break;
      case 2:
        glow_color_seed = random(255);
        break;
      case 3:
        simm_leds_counter = random(255);
        simm_leds_shift = random(-10, 10);
         simm_leds_time = random(50,75);
        break;
    }
    FastLED.clear();
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