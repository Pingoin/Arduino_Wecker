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
    saveConfig();
  }
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
    for (unsigned int t = 0; t < sizeof(storage); t++)
      *((char*)&storage + t) = EEPROM.read(CONFIG_START + t);
}

void saveConfig() {
  for (unsigned int t = 0; t < sizeof(storage); t++) {
    if (EEPROM.read(CONFIG_START + t) != *((char*)&storage + t)) {
      EEPROM.write(CONFIG_START + t, *((char*)&storage + t));
      Serial.print("w");
    } else {
      Serial.print("r");
    }
  }
  Serial.print("  ");
  Serial.print(sizeof(storage));
  Serial.println(" Bytes aktualisiert");
}
