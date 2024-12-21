// ===== НАСТРОЙКИ =====
#define EDGE_LEDS 24  // кол-во диодов на ребре ТЕТРАЭДРА
#define LED_DI 2      // пин подключения
#define BRIGHT 250    // яркость
#define THETIME 5     //время между смено режимов

const int NUM_LEDS = (EDGE_LEDS * 6);  //кол во светодиодов
#include <FastLED.h>                   //подключаем библиотеку
CRGB leds[NUM_LEDS];                   //инициализируем
unsigned long prewmillis = 0;          //предыдущий миллис
int shift_time = 100;                  //время обновления режимов
int bright = -1;                       //яркость
unsigned long prewrefading = 0;        //для включения затемнения
unsigned long prewfading = 0;          //для плавного затемнения
bool fadingdir = true;                 //направлени темним /светим
bool flagfading = true;                //темним или нет
int fading_counter = 0;                //счетчик для темнени
int8_t regim = -1; //режим
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
uint8_t comet_color = 0;     //цвет
uint8_t cometa_lenght = 0;  //длина

int headr = 0;                 //ярчайшая точка кометыр
int head_beginr = 0;           //пачало пути
int head_endr = 0;             //конец пути
uint8_t comet_colorr = 0;       //цвет
int shift_timer = 0;          //вроемя обновления
uint8_t cometa_lenghtr = 0;    //длина
unsigned long prewmillisr = 0;  //для таймера

uint8_t glow_color[NUM_LEDS];       // массив цветов
uint8_t glow_bright[NUM_LEDS];      //массив яркостей
uint8_t glow_color_seed = 0;    //минимальный цвет палитры
uint8_t glow_color_shift = 0;  //максимальный сдвиг

int simm_leds_color[EDGE_LEDS / 2];  //массив цветов точек от вершины в половину ребра
int simm_leds_counter = 0;           //счетчик
int8_t simm_leds_shift = 0;          //сдвиг

uint8_t perlin_zero = 0;               //начало перлина
uint8_t perlin_sat_min = 0;            //мин насыщенность
uint8_t perlin_sat_max = 0;            //мах насыщенность
uint8_t perlin_brig_min = 0;           //мин яркость
uint8_t perlin_brig_max = 0;           //мах яркость
int perlin_counter = 0;                  //счетчик перлина
int perlin_counter_zero = 0;             //обнулиеть
int perlin_counter_shift = 0;           //сдвигатель
uint8_t perlin_hue = 0;                //предел изменения цвета
uint8_t perlin_step = 0;                //шаг изменения перлина
int perlin = 0;                          //иего собвственной персоной перлин
int total_rainbow_color = 0;             //цведля радуги
int total_rainbow_shift = 0;             //сдвиг для радуги

void setup() {
  // put your setup code here, to run once:
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 6000);  //настройка напряжения и силы тока
  FastLED.addLeds<WS2812, LED_DI, GRB>(leds, NUM_LEDS)
  .setCorrection(TypicalLEDStrip);    //инициация ленты
  FastLED.setBrightness(bright);        //установка яркости
  leds[0].setRGB(100, 100, 255);        //первый свтеодиод серый => работает
  FastLED.show();                       //выводим
  delay(1000);                          //смотрим
  FastLED.clear();                      //обнуляем
  FastLED.show();                       //выводим
  randomSeed(analogRead(A0));           //рандомимм зерно рандома
  for (int i = 0; i < NUM_LEDS; i++) {  //приравниваем все значения массивов к нулю
    glow_bright[i] = 0;
  }
  for (int i = 0; i < EDGE_LEDS / 2; i++) {
    simm_leds_color[i] = 0;
  }
}

void loop() {  //лупим
  // put your main code here, to run repeatedly:
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
    FastLED.show();  //выводим
  }
  if (millis() - prewrefading > THETIME * 1000) {  //проверяем не пора ли делать следующее
    flagfading = true;                             //включаем процесс
    fadingdir = true;                              //велючаем направление в темную
    prewrefading = millis();
  }
  if (flagfading) {
    fading();  //если надо темнить темним
  }
}  //залупили



void total_rainbow() {                                                                                   //режимы не из вершин
  for (int i = 0; i < NUM_LEDS; i++) {  // все точки текущего цвета радуги
    leds[i].setHSV(total_rainbow_color, 255, 200);
  }
  total_rainbow_color += total_rainbow_shift;  //сдвигаем цвет текущей радуги
}

void total_one_color_perlin() {
  perlin = inoise8(perlin_step, perlin_counter);                      //я сам не знаю, что это такое
  perlin_counter = perlin_counter_zero;                               //счетчик приравниваем к начальному значению счетчика
  for (int i = 0; i < NUM_LEDS; i++) {                                //каждой точке приравниваем значение текущего перлина
    perlin_leds(i, perlin_sat_min, perlin_sat_max, perlin_brig_min, perlin_brig_max);
  }
  perlin_counter_zero += perlin_counter_shift;  //двигаем начальное значение счетчика(в след раз при забывании сдвинем)
}

void total_perlin() {
  for (int i = 0; i < NUM_LEDS; i++) {                                                         //все заливаем по перлину
    perlin = inoise8(i * perlin_step, perlin_counter);                                         //не спрашивайте
    perlin_leds(i, perlin_sat_min, perlin_sat_max, perlin_brig_min, perlin_brig_max);
    perlin_counter += perlin_counter_shift;                                                    //увеличиваем значение счетчика на сдвиг (мы сдвигаем каждый)
  }
  perlin_counter = perlin_counter_zero;
  perlin_counter_zero += perlin_counter_shift;  //двигаем начальное значение счетчика(в след раз при забывании сдвинем)
}

void simm_rainbow() {
  for (int i = 0; i < EDGE_LEDS / 2; i++) {                        //чтука для радуги
    simm_leds_color[i] = simm_leds_shift * i + simm_leds_counter;  //каждая точка - сдвиг + движущийся счетчик
    simm_leds_counter -= 1;                                        //двигаем счетщик
  }
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < EDGE_LEDS / 2; j++) {                                  //ввод радуги
      leds[i * EDGE_LEDS + j].setHSV(simm_leds_color[j], 255, 200);            // с одной стороны
      leds[(i + 1) * EDGE_LEDS - j - 1].setHSV(simm_leds_color[j], 255, 200);  // и с другой
    }
  }
}
void simm_perlin() {
  for (int i = 0; i < 6; i++) {                                                           //для каждой из сторон
    for (int j = EDGE_LEDS / 2 - 1; j >= 0; j--) {                                      //для каждой половинки ребра
      perlin = inoise8(j * perlin_step, perlin_counter);                                // напишите сюда,если узнаете, как это работает
      perlin_leds(i * EDGE_LEDS + j, perlin_sat_min, perlin_sat_max, perlin_brig_min, perlin_brig_max);
      perlin_leds((i + 1) * EDGE_LEDS - j - 1, perlin_sat_min, perlin_sat_max, perlin_brig_min, perlin_brig_max);
      perlin_counter += perlin_counter_shift;  //двигаем счетчик, чтобы цвета менялись от точки к точке
    }
    perlin_counter = perlin_counter_zero;  //забываем про изменения счетчика
    perlin_counter_zero += perlin_counter_shift;  //двигаем забвение
  }
}
void simm_cometa() {
  for (int i = 0; i < 6; i++) {      //режимы из вершин
    for (int j = 0; j < EDGE_LEDS / 2; j++) {             //для каждого ребра
      perlin = inoise8(j * perlin_step, perlin_counter);  //я уверен эта строчка не нуждается в моих объяснениях
      perlin_leds(i * EDGE_LEDS + j, perlin_sat_min - j * 10, perlin_sat_max - j * 10, perlin_brig_min, perlin_brig_max - 1.5 * j * j);//цвет как обычно //насыщенность уменьшаем
      perlin_leds((i + 1) * EDGE_LEDS - j - 1, perlin_sat_min - j * 10, perlin_sat_max - j * 10, perlin_brig_min, perlin_brig_max - 1.5 * j * j);//яркость уменьшаем по параболе
      perlin_counter += perlin_counter_shift;  //двигаем счетчик, чтобы цвета менялись от точки к точке
    }
    perlin_counter = perlin_counter_zero;                                      //забываем про изменения счетчика
  }
  perlin_counter_zero += perlin_counter_shift;  //двигаем забвение
}

void glowing() {                     //режимы светлячков
  for (int i = 0; i < random(1,3); i++) {       //учащаем появление
    int randglow = random(NUM_LEDS);  //выбираем случайного сверчка
    glow_bright[randglow] = 255;      //врубаем его
    if (glow_color_seed == 0) {       //рандомим цвет(не по палитре)
      glow_color[randglow] = random(255);
    } else {  // а здесь по палитре
      glow_color[randglow] = glow_color_seed + random(glow_color_shift);
    }
  }
  for (int i = 0; i < NUM_LEDS; i++) {  //тушим каждого слегка
    if (glow_bright[i] > 0) {
      glow_bright[i] -= 10;
      if (glow_bright[i] < 0) {  //если перетушили, вырубаем
        glow_bright[i] = 0;
      }
      leds[i].setHSV(glow_color[i], 255, glow_bright[i]);  //вводим
    }
  }
}
void cometa() {                            //комета нормальная
  if (head - cometa_lenght == head_end) {  //если комета полностью зашла
    head = random(0, 6) * EDGE_LEDS;       //рандомим светлейшую точку
    head_end = head + EDGE_LEDS;           //и конец
    head_begin = head - 1;                 //и начало
    shift_time = random(50, 90);           //и время
    comet_color = random(0, 255);          //и цвет
    cometa_lenght = random(3, 6) + random(4, 7);       //и длину
  }
  for (int i = 0; i < cometa_lenght; i++) {                                                                      //обновляем комету
    if ((head - i < head_end) and (head - i > head_begin) and (head - i >= 0)) {                                //только те точки, которые между началом и концом и существуют
      leds[head - i].setHSV(comet_color, 255 - i * (255 / (cometa_lenghtr + 4)), 255 - i * (255 / cometa_lenght)); //конец менее яркий и насыщенный
    }
  }
  if (head - cometa_lenght - 1 >= 0) {
    leds[head - cometa_lenght].setRGB(0, 0, 0);  //закрашиваем хвост
  }
  head += 1;                                   //двигаем
}
void cometaReverced() {                       //комета ненормальная
  if (headr + cometa_lenghtr == head_endr) {  //выполняем действия, ровно аналогичные вышеописанным
    headr = random(1, 6) * EDGE_LEDS;
    head_endr = headr - EDGE_LEDS - 1;
    head_beginr = headr;
    shift_timer = random(50, 90);
    comet_colorr = random(0, 255);
    cometa_lenghtr = random(4, 7) + random(4, 7);
  }
  for (int i = 0; i < cometa_lenghtr; i++) {
    if ((headr + i > head_endr) and (headr + i < head_beginr) and (headr + i >= 0) ) {
      leds[headr + i].setHSV(comet_colorr, 255 - i * (255 / (cometa_lenghtr + 4)), 255 - i * (255 / cometa_lenghtr));
    }
  }
  leds[headr + cometa_lenghtr].setRGB(0, 0, 0);
  headr -= 1;
}

void fading() {         //темним
  if (bright <= 0) {    //если дотемнили
    fadingdir = false;  //вклюаем на светлую
    for (int i = 0; i < NUM_LEDS; i++) {
      glow_bright[i] = 0;  //числим светлячков
    }
    regim = random(9);                      //рандомим режим
    switch (regim) {                        //и соответственно его выбираем
      case 0:                               //кометы
        //реверснутая
        headr = random(1, 6) * EDGE_LEDS;   //выбираем ярчайшую точку
        head_endr = headr - EDGE_LEDS - 1;  //куда идем
        head_beginr = headr;                //откуда идем
        shift_timer = random(40, 100);      //рандомим скорость
        comet_colorr = random(0, 255);      //рандомим цвет
        cometa_lenghtr = random(8, 14);     //рандомим длину
        //адекватная
        head = random(0, 6) * EDGE_LEDS;    //ну тут все тоже самое
        head_end = head + EDGE_LEDS;
        head_begin = head - 1;
        shift_time = random(50, 90);
        comet_color = random(0, 255);
        cometa_lenght = random(8, 14);
        break;
      case 1:                         //светлячки
        glow_color_seed = 0;          //когда зерно 0 идут все цвета
        shift_time = random(50, 90);  // рандомим время
        break;
      case 2:                           //светлячки по палитре
        glow_color_seed = random(255);  //рандомим основной цвет палитры
        glow_color_shift = random(20, 60);
        shift_time = random(50, 90);  //сами догадайтесь
        break;
      case 3:                               //радуга из вершин
        simm_leds_counter = random(255);    //в счете движенье
        simm_leds_shift = random(-10, 10);  //на сколько отличается цвет одного от другово
        shift_time = random(50, 90);        //в комментариях не нуждается
        break;
      case 4:                                            //перлин из вершин
        perlin_zero = random(255);                       //начальный цвет
        perlin_brig_max = random(100, 255);              //мах яркость
        perlin_brig_min = random(100, perlin_brig_max);  //мин ярк
        perlin_sat_max = random(200, 255);               //мах насыщенность
        perlin_sat_min = random(200, perlin_sat_max);    //мин насыщ
        perlin_counter = 0;                              //счетчик перлина обнуляем
        perlin_counter_zero = 0;                         //счетчик движения нулевого перлина обнуляем
        perlin_counter_shift = random(1, 17);            //счетчик перепада цветов рандомим
        perlin_hue = random(125, 200);                   //на сколько сильно меняется цвет
        perlin_step = random(1, 17);                     //как быстро меняем перлина
        shift_time = random(50, 75);  //думаю, что несмотря на большое количество комментариев, вы все еще помните что это
        break;
      case 5:                       //перлин затухая из вершин
        perlin_zero = random(255);  //смотри саsе 4
        perlin_brig_max = random(200, 255);
        perlin_brig_min = 0;
        perlin_sat_max = random(220, 255);
        perlin_sat_min = random(220, perlin_sat_max);
        perlin_counter = 0;
        perlin_counter_zero = 0;
        perlin_counter_shift = 1;      // сдвиг перлина  с каждым разом
        perlin_hue = random(90, 200);  //добавление hue к зеро
        perlin_step = random(5, 40);   //скорость изменения цвета
        shift_time = random(40, 50);  //время обновления
        break;
      case 6:                               //перлин по всей штуке
        perlin_zero = random(255);          //дальше можешь перечитать саsе5
        perlin_brig_max = random(200, 255);
        perlin_brig_min = random(100, perlin_brig_max);
        perlin_sat_max = random(220, 255);
        perlin_sat_min = random(200, perlin_sat_max);
        perlin_counter = 0;
        perlin_counter_zero = 0;
        perlin_counter_shift = random(10, 35);  // сдвиг перлина  с каждым разом
        perlin_hue = random(100, 200);          //добавление hue к зеро
        perlin_step = random(10, 40);           //скорость изменения цвета
        shift_time = random(40, 60);            //*кончилась фантазия*
        break;
      case 7:                              //перлин но одним цветом вся штука
        perlin_zero = random(255);         //я думаю вы знаете, где это смотреть
        perlin_brig_max = random(200, 255);
        perlin_brig_min = random(100, perlin_brig_max);
        perlin_sat_max = random(220, 255);
        perlin_sat_min = random(200, perlin_sat_max);
        perlin_counter = 0;
        perlin_counter_zero = 0;
        perlin_counter_shift = random(5, 35);  // сдвиг перлина  с каждым разом
        perlin_hue = random(100, 250);         //random(90, 200);  //добавление hue к зеро
        perlin_step = random(10, 40);          // random(5, 40);   //скорость изменения цвета
        shift_time = random(40, 60);           //*_*
        break;
      case 8:                                  //вся одним по радуге цветом
        total_rainbow_color = random(0, 255);  //задаем начальный цвет
        total_rainbow_shift = random(1, 10);   //задаем сдвиг
        shift_time = random(35, 50);           //*^*
        break;
    }

    FastLED.clear();     //чистим
    FastLED.show();      //выводим
    fading_counter = 0;  //рестартим счетчик
    delay(250);          //театральная пауза
  }
  if (fadingdir) {                        //если темним
    if (millis() - prewfading > 10) {     //(темнить надо по времени)
      bright -= int(fading_counter ^ 2);  //уменьшаем яркость на квадрат счетчика (пародия на срт)
      fading_counter += 0.5;              //увеличиваем счетчик
      prewfading = millis();
    }
  } else {                   //если светлим
    if (bright >= BRIGHT) {  //если досветлили
      flagfading = false;    //вырубаем темнение
      fading_counter = 0;    //вырубаем счетчик
    }
    if (millis() - prewfading > 10) {     //если светлим
      bright += int(fading_counter ^ 2);  //увеличиваем яркость на квадрат счетчика (пародия на срт)
      fading_counter += 0.5;              //увеличиваем счетчик
      prewfading = millis();
    }
  }
  FastLED.setBrightness(bright);  //обновляем яркость ( а то зря что ли ее изменяем)
}

void perlin_leds(int num, int sat_min, int sat_max, int brig_min, int brig_max) { //функция для ввода на ленту значений перлина
  leds[num].setHSV(perlin_zero + map(perlin, 0, 255, 0, perlin_hue),  //выше
                   constrain(map(perlin, 0, 255, sat_min, sat_max), 0, 255),
                   constrain(map(perlin, 0, 255, brig_min, brig_max), 0, 255));
}


//границы адекватных значаний (ГАЗ)

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
