// ===== НАСТРОЙКИ =====
#define EDGE_LEDS 24       // кол-во диодов на ребре ТЕТРАЭДРА
#define LED_DI 5           // пин подключения
int changing_time = 10;     //дефолтное время между смено режимов
#define BTN_PIN 4          //пин кнопки
#define MAX_AMP 6          //максимальный ток бп
#define numHold_Time 1500  // время отображения индикатора уровня яркости/скорости/масштаба


#include <GyverButton.h>
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);  // если не сенсорная кнопка-изменить

uint8_t gaz[7][14] = {
  //gраницы aдакватных zначений
  {
    //комета
    45,  //скорость
    95,
    8,  //длина
    14,
  },
  {
    // мерцание
    50,  //скорость
    90,
    20,  //сдвиг палитры
    60,
  },
  {
    //симмрадуга
    50,  //скорость
    90,
    10,  //сдвиг(1),
  },
  {
    //перлин из вершин
    50,  //скорость
    75,
    100,  //мах яркость
    255,
    100,  //мин яркость(1)
    200,  //мах насыщенность,
    255,
    200,  //,мин насыщенность(1)
    1,    //счетчик перепада цветов
    17,
    125,  //на сколько сильно меняется цвет
    200,
    1,  //,как быстро меняем перлина
    17,
  },
  {
    //перлин затухая из вершин
    40,  //скорость
    50,
    200,  //мах яркость
    255,
    220,  //мах насыщенность
    255,
    220,  //,мин насыщенность(1),
    1,    //счетчик перепада цветов,
    17,
    90,  //на сколько сильно меняется цвет
    200,
    5,  //,как быстро меняем перлина,
    40,
  },
  {
    //перлин по всему,одним цветом по перлину
    40,  //скорость
    60,
    200,  //мах яркость,
    255,
    100,  //мин яркость(1)
    220,  //мах насыщенность
    255,
    200,  //мин насыщенность(1)
    10,   //счетчик перепада цветов
    35,
    100,  //на сколько сильно меняется цвет
    200,
    10,  //как быстро меняем перлина
    40,
  },
  {
    //одним цветом по радуге
    35,  //скорость
    50,
    10,  //сдвиг радуги(1),
  },
};

uint8_t max_bright = 250;              // яркость
const int NUM_LEDS = (EDGE_LEDS * 6);  //кол во светодиодов
#include <FastLED.h>                   //подключаем библиотеку
CRGB leds[NUM_LEDS];                   //инициализируем

unsigned long numHold_Timer = 0;
byte numHold;
bool max_bright_changing_dir = 0;
bool changing_time_dir = 1;
unsigned long prewmillis = 0;  //предыдущий миллис
bool is_working = true;
bool stop_working = false;
int shift_time = 100;            //время обновления режимов
int bright = -1;                 //яркость
unsigned long prewrefading = 0;  //для включения затемнения
unsigned long prewfading = 0;    //для плавного затемнения
bool fadingdir = true;           //направлени темним /светим
bool flagfading = true;          //темним или нет
int fading_counter = 0;          //счетчик для темнени
bool change_regims = 1;
int8_t regim = -1;  //режим
/*  0-кометы
  1-цвет.точки
  2точки по палитре
  3-симметричная радуга
  4 симметрия по перлину
  5 симметрия как кометы
  6 -общий перлин
  7-один цвет, меняющийся по перлину
  8 - один цвет, меняющийся по радуге
*/

int head = 0;               //ярчаяшая точка кометы
int head_begin = 0;         //начало пути
int head_end = 0;           //конец пути
uint8_t comet_color = 0;    //цвет
uint8_t cometa_lenght = 0;  //длина

int headr = 0;                  //ярчайшая точка кометыр
int head_beginr = 0;            //пачало пути
int head_endr = 0;              //конец пути
uint8_t comet_colorr = 0;       //цвет
int shift_timer = 0;            //вроемя обновления
uint8_t cometa_lenghtr = 0;     //длина
unsigned long prewmillisr = 0;  //для таймера

uint8_t glow_color[NUM_LEDS];   // массив цветов
uint8_t glow_bright[NUM_LEDS];  //массив яркостей
uint8_t glow_color_seed = 0;    //минимальный цвет палитры
uint8_t glow_color_shift = 0;   //максимальный сдвиг

int simm_leds_color[EDGE_LEDS / 2];  //массив цветов точек от вершины в половину ребра
int simm_leds_counter = 0;           //счетчик
int8_t simm_leds_shift = 0;          //сдвиг

uint8_t perlin_zero = 0;       //начало перлина
uint8_t perlin_sat_min = 0;    //мин насыщенность
uint8_t perlin_sat_max = 0;    //мах насыщенность
uint8_t perlin_brig_min = 0;   //мин яркость
uint8_t perlin_brig_max = 0;   //мах яркость
int perlin_counter = 0;        //счетчик перлина
int perlin_counter_zero = 0;   //обнулиеть
int perlin_counter_shift = 0;  //сдвигатель
uint8_t perlin_hue = 0;        //предел изменения цвета
uint8_t perlin_step = 0;       //шаг изменения перлина
int perlin = 0;                //иего собвственной персоной перлин
int total_rainbow_color = 0;   //цведля радуги
int total_rainbow_shift = 0;   //сдвиг для радуги

void setup() {
  // put your setup code here, to run once:
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_AMP * 1000);  //настройка напряжения и силы тока
  FastLED.addLeds<WS2812, LED_DI, GRB>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip);  //инициация ленты
  FastLED.setBrightness(bright);      //установка яркости
  leds[0].setRGB(100, 100, 255);      //первый свтеодиод серый => работает
  FastLED.show();                     //выводим
  delay(1000);                        //смотрим
  FastLED.clear();                    //обнуляем
  FastLED.show();                     //выводим
  randomSeed(analogRead(A0));         //рандомимм зерно рандома
  touch.setStepTimeout(100);
  touch.setClickTimeout(500);

  for (int i = 0; i < NUM_LEDS; i++) {  //приравниваем все значения массивов к нулю
    glow_bright[i] = 0;
  }
  for (int i = 0; i < EDGE_LEDS / 2; i++) {
    simm_leds_color[i] = 0;
  }
}

void loop() {  //лупим
  // put your main code here, to run repeatedly:
  if (is_working) {
    FastLED.show();                                                 //выводим
    if ((millis() - prewmillisr > shift_timer) and (regim == 0)) {  //реверснутая комета - особенная
      cometaReverced();
      prewmillisr = millis();
    }
    if (millis() - prewmillis > shift_time) {  //проверяем время обновления
      switch (regim) {                         //выбираем текущий режим и обновляем
        case 0: cometa(); break;
        case 1: glowing(); break;
        case 2: glowing(); break;
        case 3: simm_rainbow(); break;
        case 4: simm_perlin(); break;
        case 5: simm_cometa(); break;
        case 6: total_perlin(); break;
        case 7: total_one_color_perlin(); break;
        case 8: total_rainbow(); break;
      }
      prewmillis = millis();
    }
    if (millis() - prewrefading > changing_time * 1000 and change_regims) {  //проверяем не пора ли делать следующее
      flagfading = true;                                                     //включаем процесс
      fadingdir = true;                                                      //велючаем направление в темную
      prewrefading = millis();
    }
    if (flagfading) {
      fading();  //если надо темнить темним
    }
  }
  buttonTick();
  FastLED.setBrightness(constrain(bright, 0, max_bright));  //обновляем яркость ( после ее изменения в фейдинге и бтиккинге изменяем)
}  //залупили

void fading() {          //темним
  if (bright <= 0) {     //если дотемнили
    fadingdir = false;   //вклюаем на светлую
    fading_counter = 0;  //рестартим счетчик
    for (int i = 0; i < NUM_LEDS; i++) {
      glow_bright[i] = 0;  //числим светлячков
    }
    regim = random(9);  //рандомим режим
    switch (regim) {    //и соответственно его выбираем
      case 0:           //кометы
        //реверснутая
        headr = random(1, 6) * EDGE_LEDS;               //выбираем ярчайшую точку
        head_endr = headr - EDGE_LEDS - 1;              //куда идем
        head_beginr = headr;                            //откуда идем
        shift_timer = random(gaz[0][0], gaz[0][1]);     //рандомим скорость
        comet_colorr = random(255);                     //рандомим цвет
        cometa_lenghtr = random(gaz[0][2], gaz[0][2]);  //рандомим длину
        //адекватная
        shift_time = random(gaz[0][0], gaz[0][1]);
        head = random(0, 6) * EDGE_LEDS;  //ну тут все тоже самое
        head_end = head + EDGE_LEDS;
        head_begin = head - 1;
        comet_color = random(255);
        cometa_lenght = random(gaz[0][2], gaz[0][3]);
        break;
      case 1:                                       //светлячки
        shift_time = random(gaz[1][0], gaz[1][1]);  // рандомим время
        glow_color_seed = 0;                        //когда зерно 0 идут все цвета
        break;
      case 2:                                       //светлячки по палитре
        shift_time = random(gaz[1][0], gaz[1][1]);  //сами догадайтесь
        glow_color_seed = random(255);              //рандомим основной цвет палитры
        glow_color_shift = random(gaz[1][2], gaz[1][3]);
        break;
      case 3:                                                 //радуга из вершин
        shift_time = random(gaz[2][0], gaz[2][1]);            //в комментариях не нуждается
        simm_leds_counter = random(255);                      //в счете движенье
        simm_leds_shift = random(-1 * gaz[2][2], gaz[2][2]);  //на сколько отличается цвет одного от другово
        break;
      case 4:                                                  //перлин из вершин
        shift_time = random(gaz[3][0], gaz[3][1]);             //думаю, что несмотря на большое количество комментариев, вы все еще помните что это
        perlin_zero = random(255);                             //начальный цвет
        perlin_brig_max = random(gaz[3][2], gaz[3][3]);        //мах яркость
        perlin_brig_min = random(gaz[3][4], perlin_brig_max);  //мин ярк
        perlin_sat_max = random(gaz[3][5], gaz[3][6]);         //мах насыщенность
        perlin_sat_min = random(gaz[3][7], perlin_sat_max);    //мин насыщ
        perlin_counter = 0;                                    //счетчик перлина обнуляем
        perlin_counter_zero = 0;                               //счетчик движения нулевого перлина обнуляем
        perlin_counter_shift = random(gaz[3][8], gaz[3][9]);   //счетчик перепада цветов рандомим
        perlin_hue = random(gaz[3][10], gaz[3][11]);           //на сколько сильно меняется цвет
        perlin_step = random(gaz[3][12], gaz[3][13]);          //как быстро меняем перлина
        break;
      case 5:                                       //перлин затухая из вершин
        shift_time = random(gaz[4][0], gaz[4][1]);  //время обновления
        perlin_zero = random(255);                  //смотри саsе 4
        perlin_brig_max = random(gaz[4][2], gaz[4][3]);
        perlin_brig_min = 0;
        perlin_sat_max = random(gaz[4][4], gaz[4][5]);
        perlin_sat_min = random(gaz[4][6], perlin_sat_max);
        perlin_counter = 0;
        perlin_counter_zero = 0;
        perlin_counter_shift = random(gaz[4][7], gaz[4][8]);  // сдвиг перлина  с каждым разом
        perlin_hue = random(gaz[4][9], gaz[4][10]);           //добавление hue к зеро
        perlin_step = random(gaz[4][11], gaz[4][12]);         //скорость изменения цвета
        break;
      case 6:                                       //перлин по всей штуке
        shift_time = random(gaz[5][0], gaz[5][1]);  //*кончилась фантазия*
        perlin_zero = random(255);                  //дальше можешь перечитать саsе5
        perlin_brig_max = random(gaz[5][2], gaz[5][3]);
        perlin_brig_min = random(gaz[5][4], perlin_brig_max);
        perlin_sat_max = random(gaz[5][5], gaz[5][6]);
        perlin_sat_min = random(gaz[5][7], perlin_sat_max);
        perlin_counter = 0;
        perlin_counter_zero = 0;
        perlin_counter_shift = random(gaz[5][8], gaz[5][9]);  // сдвиг перлина  с каждым разом
        perlin_hue = random(gaz[5][10], gaz[5][11]);          //добавление hue к зеро
        perlin_step = random(gaz[5][12], gaz[5][13]);         //скорость изменения цвета
        break;
      case 7:                                       //перлин но одним цветом вся штука
        shift_time = random(gaz[5][0], gaz[5][1]);  //*_*
        perlin_zero = random(255);                  //я думаю вы знаете, где это смотреть
        perlin_brig_max = random(gaz[5][2], gaz[5][3]);
        perlin_brig_min = random(gaz[5][4], perlin_brig_max);
        perlin_sat_max = random(gaz[5][5], gaz[5][6]);
        perlin_sat_min = random(gaz[5][7], perlin_sat_max);
        perlin_counter = 0;
        perlin_counter_zero = 0;
        perlin_counter_shift = random(gaz[5][8], gaz[5][9]);  // сдвиг перлина  с каждым разом
        perlin_hue = random(gaz[5][10], gaz[5][11]);          //добавление hue к зеро
        perlin_step = random(gaz[5][12], gaz[5][13]);         //скорость изменения цвета
        break;
      case 8:                                                     //вся одним по радуге цветом
        shift_time = random(gaz[6][0], gaz[6][1]);                //*^*
        total_rainbow_color = random(255);                        //задаем начальный цвет
        total_rainbow_shift = random(-1 * gaz[6][2], gaz[6][2]);  //задаем сдвиг
        break;
    }
    FastLED.clear();  //чистим
    FastLED.show();   //выводим
  }
  if (fadingdir) {                        //если темним
    if (millis() - prewfading > 10) {     //(темнить надо по времени)
      bright -= int(fading_counter ^ 2);  //уменьшаем яркость на квадрат счетчика (пародия на срт)
      fading_counter += 0.5;              //увеличиваем счетчик
      prewfading = millis();
    }
  } else {                       //если светлим
    if (bright >= max_bright) {  //если досветлили
      flagfading = false;        //вырубаем темнение
      fading_counter = 0;        //вырубаем счетчик
    }
    if (stop_working) {
      is_working = 0;
    }
    if (millis() - prewfading > 10 and is_working) {  //если светлим
      bright += int(fading_counter ^ 2);              //увеличиваем яркость на квадрат счетчика (пародия на срт)
      fading_counter += 0.5;                          //увеличиваем счетчик
      prewfading = millis();
    }
  }
}

/* режим ядерная реакция:
  сначала кометы в две стороны, на гранях расходятся в обе возможные стороны и так по геометрической прогрессии. Лень делать
  int nuc_start_point = 0;
  int nuc_points[NUM_LEDS];
  int nuc_shift = 1;
  void nuclear() {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (nuc_points[i] > 0) {
      leds[i].setHSV(255 - nuc_points[i], nuc_points[i], nuc_points[i]);
      nuc_points[i] -= nuc_shift;
      if (nuc_points[i] < 0) {
        nuc_points[i] = 0;
      }
    }
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    if (nuc_points[i - 1] == 0 and nuc_points[i + 1] == 255 - nuc_shift) {
      nuc_points[i - 1] = 255;
    }
    if (nuc_points[i + 1] == 0 and nuc_points[i - 1] == 255 - nuc_shift) {
      nuc_points[i + 1] = 255;
    }
  }
  }*/