// ===== НАСТРОЙКИ =====
#define EDGE_LEDS 24  // кол-во диодов на ребре куба
#define LED_DI 2      // пин подключения
#define BRIGHT 250    // яркость
#define THETIME 10

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

int bright = -1;
unsigned long prewrefading = 0;
unsigned long prewfading = 0;
bool fadingdir = true;
bool flagfading = true;
int fading_counter = 0;
int regim = -1;  //0-кометы 1-цвет.точки 2точки по палитре 3-симметричная радуга 4 симметрия по перлину

unsigned long prewcreateglowig = 0;
int glow_color[NUM_LEDS];
int glow_bright[NUM_LEDS];
int glow_color_seed = 0;
int glow_color_shift = 20;
int randglow = 0;

unsigned long prewsimm = 0;
int simm_leds_color[NUM_LEDS / 2];
int simm_leds_counter = 0;
int simm_leds_shift = 0;
int simm_leds_time = 50;

int simm_per_zero = 220;
int simm_per_sat_min = 200;
int simm_per_sat_max = 255;
int simm_per_brig_min = 220;
int simm_per_brig_max = 255;
int perlin_counter = 0;
int perlin_counter_zero = 0;
int perlin_counter_shift = 10;
int simm_per_hue = 150;
int simm_per_step = 10;
int perlin = 0;
bool do_simm_perlin_reverced = 0;
bool do_simm_perlin = true;
bool do_simm_cometa = true;

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
  if ((millis() - prewmillisr > shift_timer) and (regim == 0)) {
    cometaReverced();
    prewmillisr = millis();
  }
  if (millis() - prewmillis > shift_time) {
    switch (regim) {
      case 0:
        cometa();
        break;
      case 1:
        glowing();
        break;
      case 2:
        glowing();
        break;
      case 3:
        simmetr();
        break;
      case 4:
        simmetr();
        break;
      case 5:
        simmetr();
        break;
    }
    prewmillis = millis();
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

void total_perlin() {
}

void simmetr() {
  for (int i = 0; i < EDGE_LEDS / 2; i++) {

    simm_leds_color[i] = simm_leds_shift * i + simm_leds_counter;
    simm_leds_counter -= 1;
  }
  for (int i = 0; i < 6; i++) {
    if (do_simm_perlin) {
      if (do_simm_perlin_reverced) {
        for (int j = 0; j < EDGE_LEDS / 2; j++) {

          perlin = inoise8(j * simm_per_step, perlin_counter);
          leds[i * EDGE_LEDS + j].setHSV(simm_per_zero + map(perlin, 0, 255, 0, simm_per_hue),
                                         constrain(map(perlin, 0, 255, simm_per_sat_min, simm_per_sat_max), 0, 255),
                                         constrain(map(perlin, 0, 255, simm_per_brig_min, simm_per_brig_max), 0, 255));
          leds[(i + 1) * EDGE_LEDS - j - 1].setHSV(simm_per_zero + map(perlin, 0, 255, 0, simm_per_hue),
                                                   constrain(map(perlin, 0, 255, simm_per_sat_min, simm_per_sat_max), 0, 255),
                                                   constrain(map(perlin, 0, 255, simm_per_brig_min, simm_per_brig_max), 0, 255));
          perlin_counter += perlin_counter_shift;
        }
        perlin_counter = perlin_counter_zero;
      } else {
        for (int j = EDGE_LEDS / 2 - 1; j >= 0; j--) {
          perlin = inoise8(j * simm_per_step, perlin_counter);
          leds[i * EDGE_LEDS + j].setHSV(simm_per_zero + map(perlin, 0, 255, 0, simm_per_hue),
                                         constrain(map(perlin, 0, 255, simm_per_sat_min, simm_per_sat_max), 0, 255),
                                         constrain(map(perlin, 0, 255, simm_per_brig_min, simm_per_brig_max), 0, 255));
          leds[(i + 1) * EDGE_LEDS - j - 1].setHSV(simm_per_zero + map(perlin, 0, 255, 0, simm_per_hue),
                                                   constrain(map(perlin, 0, 255, simm_per_sat_min, simm_per_sat_max), 0, 255),
                                                   constrain(map(perlin, 0, 255, simm_per_brig_min, simm_per_brig_max), 0, 255));
          perlin_counter += perlin_counter_shift;
        }
        perlin_counter = perlin_counter_zero;
      }
      perlin_counter_zero += perlin_counter_shift;
    } else {
      if (do_simm_cometa) {
        for (int j = 0; j < EDGE_LEDS / 2; j++) {

          perlin = inoise8(j * simm_per_step, perlin_counter);
          leds[i * EDGE_LEDS + j].setHSV(simm_per_zero + map(perlin, 0, 255, 0, simm_per_hue),
                                         constrain(map(perlin, 0, 255, simm_per_sat_min - j * 10, simm_per_sat_max - j * 10), 0, 255),
                                         constrain(map(perlin, 0, 255, simm_per_brig_min, simm_per_brig_max -1.5* j * j), 0, 255));
          leds[(i + 1) * EDGE_LEDS - j - 1].setHSV(simm_per_zero + map(perlin, 0, 255, 0, simm_per_hue),
                                                   constrain(map(perlin, 0, 255, simm_per_sat_min - j * 10, simm_per_sat_max - j * 10), 0, 255),
                                                   constrain(map(perlin, 0, 255, simm_per_brig_min, simm_per_brig_max - 1.5*j * j), 0, 255));
          perlin_counter += perlin_counter_shift;
        }
        perlin_counter_zero += perlin_counter_shift;
      } else {
        for (int j = 0; j < EDGE_LEDS / 2; j++) {
          leds[i * EDGE_LEDS + j].setHSV(simm_leds_color[j], 255, 200);
          leds[(i + 1) * EDGE_LEDS - j - 1].setHSV(simm_leds_color[j], 255, 200);
        }
      }
      perlin_counter_zero += perlin_counter_shift;
    }
  }
  FastLED.show();
}

void glowing() {
  for (int i = 0; i < 3; i++) {
    randglow = random(NUM_LEDS);
    glow_bright[randglow] = 255;
    if (glow_color_seed == 0) {
      glow_color[randglow] = random(255);
    } else {
      glow_color[randglow] = glow_color_seed + random(-glow_color_shift, glow_color_shift);
    }
    leds[randglow].setHSV(glow_color[randglow], 255, glow_bright[randglow]);
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
    regim = random(6);
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
        shift_time = random(50, 90);
        break;
      case 2:
        glow_color_seed = random(255);
        shift_time = random(50, 90);
        break;
      case 3:
        simm_leds_counter = random(255);
        simm_leds_shift = random(-10, 10);
        shift_time = random(50, 90);
        do_simm_perlin = false;
        break;
      case 4:
        simm_per_zero = random(255);
        simm_per_brig_max = random(100, 255);
        simm_per_brig_min = random(100, simm_per_brig_max);
        simm_per_sat_max = random(200, 255);
        simm_per_sat_min = random(200, simm_per_sat_max);
        perlin_counter = 0;
        perlin_counter_zero = 0;
        perlin_counter_shift = random(1, 17);
        simm_per_hue = random(125, 200);
        simm_per_step = random(1, 17);

        do_simm_perlin = true;
        do_simm_cometa = false;
        do_simm_perlin_reverced = random(0, 2);
        shift_time = random(50, 75);
        break;
      case 5:
        simm_per_zero = random(255);
        simm_per_brig_max = random(200, 255);
        simm_per_brig_min = 0;
        simm_per_sat_max = random(220, 255);
        simm_per_sat_min = random(220, simm_per_sat_max);
        perlin_counter = 0;
        perlin_counter_zero = 0;
        perlin_counter_shift = 1;        // сдвиг перлина  с каждым разом
        simm_per_hue = random(90, 200);  //добавление hue к зеро
        simm_per_step = random(5, 40);   //скорость изменения цвета

        do_simm_cometa = true;
        do_simm_perlin = false;
        shift_time = random(40, 60);
        break;
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      glow_bright[i] = 0;
    }
    FastLED.clear();
    FastLED.show();
    fading_counter = 0;
    delay(250);
  }
  if (fadingdir) {
    if (millis() - prewfading > 10) {
      bright -= int(fading_counter ^ 2);
      fading_counter += 0.5;
      prewfading = millis();
    }
  } else {
    if (bright >= BRIGHT) {
      flagfading = false;
      fading_counter = 0;
    }
    if (millis() - prewfading > 10) {
      bright += int(fading_counter ^ 2);
      fading_counter += 0.5;
      prewfading = millis();
    }
  }
  FastLED.setBrightness(bright);
}
//границы адекватных значаний (ГАЗ)