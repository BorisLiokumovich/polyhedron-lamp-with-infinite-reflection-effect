//тут все режимы

void perlin_leds(int num, int sat_min, int sat_max, int brig_min, int brig_max) {  //функция для ввода на ленту значений перлина
  leds[num].setHSV(perlin_zero + map(perlin, 0, 255, 0, perlin_hue),               //выше
                   constrain(map(perlin, 0, 255, sat_min, sat_max), 0, 255),
                   constrain(map(perlin, 0, 255, brig_min, brig_max), 0, 255));
}


//границы адекватных значаний (ГАЗ)

void total_rainbow() {                  //режимы не из вершин
  for (int i = 0; i < NUM_LEDS; i++) {  // все точки текущего цвета радуги
    leds[i].setHSV(total_rainbow_color, 255, 200);
  }
  total_rainbow_color += total_rainbow_shift;  //сдвигаем цвет текущей радуги
}

void total_one_color_perlin() {
  perlin = inoise8(perlin_step, perlin_counter);  //я сам не знаю, что это такое
  perlin_counter = perlin_counter_zero;           //счетчик приравниваем к начальному значению счетчика
  for (int i = 0; i < NUM_LEDS; i++) {            //каждой точке приравниваем значение текущего перлина
    perlin_leds(i, perlin_sat_min, perlin_sat_max, perlin_brig_min, perlin_brig_max);
  }
  perlin_counter_zero += perlin_counter_shift;  //двигаем начальное значение счетчика(в след раз при забывании сдвинем)
}

void total_perlin() {
  for (int i = 0; i < NUM_LEDS; i++) {                  //все заливаем по перлину
    perlin = inoise8(i * perlin_step, perlin_counter);  //не спрашивайте
    perlin_leds(i, perlin_sat_min, perlin_sat_max, perlin_brig_min, perlin_brig_max);
    perlin_counter += perlin_counter_shift;  //увеличиваем значение счетчика на сдвиг (мы сдвигаем каждый)
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
  for (int i = 0; i < 6; i++) {                           //для каждой из сторон
    for (int j = EDGE_LEDS / 2 - 1; j >= 0; j--) {        //для каждой половинки ребра
      perlin = inoise8(j * perlin_step, perlin_counter);  // напишите сюда,если узнаете, как это работает
      perlin_leds(i * EDGE_LEDS + j, perlin_sat_min, perlin_sat_max, perlin_brig_min, perlin_brig_max);
      perlin_leds((i + 1) * EDGE_LEDS - j - 1, perlin_sat_min, perlin_sat_max, perlin_brig_min, perlin_brig_max);
      perlin_counter += perlin_counter_shift;  //двигаем счетчик, чтобы цвета менялись от точки к точке
    }
    perlin_counter = perlin_counter_zero;         //забываем про изменения счетчика
    perlin_counter_zero += perlin_counter_shift;  //двигаем забвение
  }
}
void simm_cometa() {
  for (int i = 0; i < 6; i++) {                                                                                                                    //режимы из вершин
    for (int j = 0; j < EDGE_LEDS / 2; j++) {                                                                                                      //для каждого ребра
      perlin = inoise8(j * perlin_step, perlin_counter);                                                                                           //я уверен эта строчка не нуждается в моих объяснениях
      perlin_leds(i * EDGE_LEDS + j, perlin_sat_min - j * 10, perlin_sat_max - j * 10, perlin_brig_min, perlin_brig_max - 1.5 * j * j);            //цвет как обычно //насыщенность уменьшаем
      perlin_leds((i + 1) * EDGE_LEDS - j - 1, perlin_sat_min - j * 10, perlin_sat_max - j * 10, perlin_brig_min, perlin_brig_max - 1.5 * j * j);  //яркость уменьшаем по параболе
      perlin_counter += perlin_counter_shift;                                                                                                      //двигаем счетчик, чтобы цвета менялись от точки к точке
    }
    perlin_counter = perlin_counter_zero;  //забываем про изменения счетчика
  }
  perlin_counter_zero += perlin_counter_shift;  //двигаем забвение
}

void glowing() {                            //режимы светлячков
  for (int i = 0; i < random(1, 3); i++) {  //учащаем появление
    int randglow = random(NUM_LEDS);        //выбираем случайного сверчка
    glow_bright[randglow] = 255;            //врубаем его
    if (glow_color_seed == 0) {             //рандомим цвет(не по палитре)
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
void cometa() {                                   //комета нормальная
  if (head - cometa_lenght == head_end) {         //если комета полностью зашла
    head = random(0, 6) * EDGE_LEDS;              //рандомим светлейшую точку
    head_end = head + EDGE_LEDS;                  //и конец
    head_begin = head - 1;                        //и начало
    shift_time = random(50, 90);                  //и время
    comet_color = random(0, 255);                 //и цвет
    cometa_lenght = random(3, 6) + random(4, 7);  //и длину
  }
  for (int i = 0; i < cometa_lenght; i++) {                                                                         //обновляем комету
    if ((head - i < head_end) and (head - i > head_begin) and (head - i >= 0)) {                                    //только те точки, которые между началом и концом и существуют
      leds[head - i].setHSV(comet_color, 255 - i * (255 / (cometa_lenghtr + 4)), 255 - i * (255 / cometa_lenght));  //конец менее яркий и насыщенный
    }
  }
  if (head - cometa_lenght - 1 >= 0) {
    leds[head - cometa_lenght].setRGB(0, 0, 0);  //закрашиваем хвост
  }
  head += 1;  //двигаем
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
    if ((headr + i > head_endr) and (headr + i < head_beginr) and (headr + i >= 0)) {
      leds[headr + i].setHSV(comet_colorr, 255 - i * (255 / (cometa_lenghtr + 4)), 255 - i * (255 / cometa_lenghtr));
    }
  }
  leds[headr + cometa_lenghtr].setRGB(0, 0, 0);
  headr -= 1;
}