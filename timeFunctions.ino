void printTimeSet(char const aktiv[3]) {
  sprintf(lcdChar, "neu:%02d:%02d:%02d %s", tm.Hour, tm.Minute, secondNew, aktiv);
  lcd.print(lcdChar);
  lcd.setCursor(0, 3);
  sprintf(lcdChar, "    %02d.%02d.%04d    ", tm.Day, tm.Month, tm.Year + 1970);
  lcd.print(lcdChar);
}

void setNewTime() {
  if (okay.checkPressed()) {
    tm.Second = (second() + 60 - secondOffset) % 60;
    t = makeTime(tm);
    RTC.set(t);
    setTime(t);
    lcd.clear();
    menuID = 0;
    secondOffset = 0;
  }
}

void printTime() {
  sprintf(lcdChar, " %02d:%02d:%02d %03s %02d.%02d.   ", hour(), minute(), second(), weckzeiten[0].createStr(weekday()), day(), month());
  lcd.print(lcdChar);
}

void sommerZeit() {
  bool DST = false;
  if (month() <= 2 || month() >= 11)
    DST = false;                                   // Winter months
  if (month() >= 4 && month() <= 9)
    DST = true;                                    // Summer months
  if (month() == 3 && (day() - (weekday() - 1)) >= 25) { // Begin of summer time
    if (hour() >= 2 || weekday() != 1)
      DST = true;
  }
  if (month() == 10 && day() - (weekday() - 1) < 25)
    DST = true;
  if (month() == 10 && day() - (weekday() - 1) >= 25) {
    if ((hour() >= 3) || weekday() != 1 || (hour() == 2 && !storage.sommer)) {
      DST = false;
    }
    else {
      DST = true;
    }
  }
  if (DST && !storage.sommer) {
    tm.Hour = hour() + 1;
    tm.Minute = minute();
    tm.Second = second();
    tm.Day = day();
    tm.Month = month();
    tm.Year = CalendarYrToTm(year());
    t = makeTime(tm);
    RTC.set(t);
    setTime(t);
    storage.sommer = true;
    saveConfig();
  } else if (!DST && storage.sommer) {
    tm.Hour = hour() - 1;
    tm.Minute = minute();
    tm.Second = second();
    tm.Day = day();
    tm.Month = month();
    tm.Year = CalendarYrToTm(year());
    t = makeTime(tm);
    RTC.set(t);
    setTime(t);
    storage.sommer = false;
    saveConfig();
  }

}

