void buttonTick() {
  touch.tick();

  if (touch.isSingle()) {
    if (is_working) {
      prewrefading = 0;
      stop_working = 1;
    } else {
      bright = 0;
      flagfading = true;
      fadingdir = false;
      prewrefading = millis();
      is_working = 1;
      stop_working = 0;
    }
  }
  if (is_working) {
    if (touch.isDouble()) {
      prewrefading = 0;
    }
    if (touch.isTriple()) {
      change_regims = !change_regims;
    }
    if ((touch.hasClicks()) && (touch.getClicks() == 5)) {}  // если было пятикратное нажатие на кнопку, то производим сохранение параметров // && (touch.hasClicks())
    if (touch.isHolded()) {
      max_bright_changing_dir = !max_bright_changing_dir;
      numHold = 1;
    }
    // if (touch.isHolded2()) {
    //    numHold = 2;
    //    changing_time_dir = !changing_time_dir;
    //   }
    //  if (touch.isHolded3()) { }
    if (touch.isStep()) {
      if (numHold != 0) numHold_Timer = millis();
      switch (numHold) {
        case 1:
          max_bright = constrain(max_bright + (2 * max_bright_changing_dir - 1) * 2, 5, 255);
          bright = max_bright;
          break;
        case 2:
          changing_time = constrain(changing_time + (2 * changing_time_dir - 1) * 2, 3, 300);
          prewrefading = millis();
          break;
      }
    }
    if ((millis() - numHold_Timer) > numHold_Time) {
      numHold = 0;
      numHold_Timer = millis();
    }
  }
}