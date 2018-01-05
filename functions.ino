void printTimeSet(char const aktiv[3]) {
  sprintf(lcdChar, "neu:%02d:%02d:%02d %s", tm.Hour, tm.Minute, secondNew, aktiv);
  lcd.print(lcdChar);
  lcd.setCursor(0, 3);
  sprintf(lcdChar, "    %02d.%02d.%04d    ", tm.Day, tm.Month, tm.Year + 1970);
  lcd.print(lcdChar);
}

void setNewTime() {
  if (okay.checkPressed()) {
    t = makeTime(tm);
    RTC.set(t);
    setTime(t);
    lcd.clear();
    menuID = 0;
    secondOffset = 0;
  }
}

void subMenuMax(byte subMenuMax) {
  if (subMenuID > 254) {
    subMenuID = subMenuMax; //maximale zahl
  }
  if (subMenuID > subMenuMax) { // maximale subMenu zahl
    subMenuID = 0;
  }
}

void weckentest() {
  if (second() <= 2) {
    for (int i = 0; i < anzWeck; i++) {
      if (weckzeiten[i].getAlarm(hour(), minute(), weekday())) {
        wecken = 1;
      }
    }
  }
  if (okay.checkPressed()) {
    wecken = 0;
  }
}

const char *  wochentag(byte wd ) {
  
  switch (wd) {
    case 0: 
    return "--";
    case 1:
      return "So";
    case 2:
      return "Mo";
    case 3:
      return "Di";
    case 4:
      return "Mi";
    case 5:
      return "Do";
    case 6:
      return "Fr";
    case 7:
      return "Sa";
    case 8:
      return "Wo";
    case 9:
      return "WE";
      case 10:
      return "++";
  }
}

void printTime() {
  sprintf(lcdChar, " %02d:%02d:%02d %03s %02d.%02d.   ", hour(), minute(), second(), wochentag(weekday()), day(), month());
  lcd.print(lcdChar);
}

bool softDelay(byte ID, int diff) {
  unsigned long nowMillis = millis();
  if (nowMillis < lastMillis[ID]) {
    lastMillis[ID] = nowMillis;
    return true;
  } else if (nowMillis >= lastMillis[ID] + diff) {
    lastMillis[ID] = nowMillis;
    return true;
  } else {
    return false;
  }
}

void loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2])
    for (unsigned int t=0; t<sizeof(storage); t++)
      *((char*)&storage + t) = EEPROM.read(CONFIG_START + t);
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(storage); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&storage + t));
}
