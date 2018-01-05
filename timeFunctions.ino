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
    local = makeTime(tm);
    RTC.set(Zeitzone.toUTC(local));
    setTime(Zeitzone.toUTC(local));
    lcd.clear();
    menuID = 0;
    secondOffset = 0;
  }
}

void printTime() {
  local=Zeitzone.toLocal(now());
  display.showNumberDec(hour(local) * 100 + minute(local), true);
  sprintf(lcdChar, " %02d:%02d:%02d %03s %02d.%02d.   ", hour(local), minute(local), second(local), weckzeiten[0].createStr(weekday(local)), day(local), month(local));
  lcd.print(lcdChar);
}

tmElements_t getLocalTime() {
  tmElements_t temp;
  local=Zeitzone.toLocal(now());
  temp.Hour = hour(local);
  temp.Minute = minute(local);
  temp.Day = day(local);
  temp.Month = month(local);
  temp.Year = CalendarYrToTm(year(local));
  return temp;
}

