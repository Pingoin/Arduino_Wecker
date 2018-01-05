void anzeige() {

  lcd.setCursor(0, 0);
  sprintf(lcdChar, "%2s;%2s;%2s;%2s;%2s;%2s;%2s", weckzeiten[0].getString(), weckzeiten[1].getString(), weckzeiten[2].getString(), weckzeiten[3].getString(), weckzeiten[4].getString(), weckzeiten[5].getString(), weckzeiten[6].getString());
  lcd.print(lcdChar);
  switch (menuID) {
    case 0:
      if (rechts.checkPressed()) {
        subMenuID++;
        lastPot = analogRead(poti);
        lcd.clear();
      }
      if (links.checkPressed()) {
        subMenuID--;
        lastPot = analogRead(poti);
        lcd.clear();
      }
      if (menu.checkPressed()) {
        subMenuID = 0;
        lcd.clear();
      }
      lcd.setCursor(0, 1);
      printTime();

      lcd.setCursor(0, 2);
      subMenuMax(2);
      switch (subMenuID) {
        case 0:
          sprintf(lcdChar, "%3d %%; %2d%cC         ", int(dht.readHumidity()), int(dht.readTemperature()), char(223));
          lcd.print(lcdChar);
          break;
        case 1:
          lcd.print("Uhr einstellen      ");
          if (okay.checkPressed()) {
            menuID = 1;
            subMenuID = 0;
            tm.Hour = hour();
            tm.Minute = minute();
            tm.Day = day();
            tm.Month = month();
            tm.Year = CalendarYrToTm(year());
            lastPot = analogRead(poti);
            lcd.clear();
          }
          break;
        case 2:
          lcd.print("Weckzeiten          ");
          if (okay.checkPressed()) {
            menuID = 3;
            weckID = 0;
            lastPot = analogRead(poti);
            lcd.clear();
          }
          break;
      }
      break;
    case 1:
      if (rechts.checkPressed()) {
        subMenuID++;
        lastPot = analogRead(poti);
        lcd.clear();
      }
      if (links.checkPressed()) {
        subMenuID--;
        lastPot = analogRead(poti);
        lcd.clear();
      }
      if (menu.checkPressed()) {
        menuID = 0;
        lcd.clear();
      }
      lcd.setCursor(0, 1);
      printTime();
      subMenuMax(5);
      lcd.setCursor(0, 2);
      if (second() - secondOffset < 0) {
        secondNew = 60 + second() - secondOffset;
      } else {
        secondNew = second() - secondOffset;
      }
      switch (subMenuID) {
        case 0:
          if (lastPot > analogRead(poti) + 5 || lastPot < analogRead(poti) - 5) {
            tm.Hour = map(analogRead(poti), 0, 1024, 0, 24);
          }
          printTimeSet("Std");
          setNewTime();
          break;
        case 1:
          if (lastPot > analogRead(poti) + 5 || lastPot < analogRead(poti) - 5) {
            tm.Minute = map(analogRead(poti), 0, 1024, 0, 61);
          }
          printTimeSet("Min");
          setNewTime();
          break;
        case 2:
          if (okay.checkPressed()) {
            secondOffset = second();
          }
          printTimeSet("Sek");
          break;
        case 3:
          if (lastPot > analogRead(poti) + 5 || lastPot < analogRead(poti) - 5) {
            tm.Day = map(analogRead(poti), 0, 1024, 1, 32);
          }
          printTimeSet("Tag");
          setNewTime();
          break;
        case 4:
          if (lastPot > analogRead(poti) + 5 || lastPot < analogRead(poti) - 5) {
            tm.Month = map(analogRead(poti), 0, 1024, 1, 13);
          }
          printTimeSet("Mon");
          setNewTime();
          break;
        case 5:
          if (lastPot > analogRead(poti) + 5 || lastPot < analogRead(poti) - 5) {
            tm.Year = CalendarYrToTm(map(analogRead(poti), 0, 1024, 1999, 2051));
          }
          printTimeSet("Jah");
          setNewTime();
          break;
      }
      break;
    case 3:
      if (rechts.checkPressed()) {
        subMenuID++;
        lastPot = analogRead(poti);
        lcd.clear();
      }
      if (links.checkPressed()) {
        subMenuID--;
        lastPot = analogRead(poti);
        lcd.clear();
      }
      if (menu.checkPressed()) {
        menuID = 0;
        lcd.clear();
      }

      if (okay.checkPressed()) {
        weckID = subMenuID;
        menuID = 4;
        subMenuID = 0;
        weckStunde = weckzeiten[weckID].getHH();
        weckMinute = weckzeiten[weckID].getMM();
        if (weckzeiten[weckID].getActive()) {
          weckTag = weckzeiten[weckID].getWD();
        } else {
          weckTag = 10;
        }
      }
      subMenuMax(6);
      lcd.setCursor(subMenuID * 3, 1);
      lcd.print("++");
      lcd.setCursor(0, 2);
      printTime();

      break;
    case 4:
      if (rechts.checkPressed()) {
        subMenuID++;
        lastPot = analogRead(poti);
        lcd.clear();
      }
      if (links.checkPressed()) {
        subMenuID--;
        lastPot = analogRead(poti);
        lcd.clear();
      }
      if (menu.checkPressed()) {
        menuID = 3;
        lcd.clear();
      }
      subMenuMax(2);
      lcd.setCursor(weckID * 3, 1);
      lcd.print("++");
      lcd.setCursor(0, 2);
      printTime();
      switch (subMenuID) {
        case 0:
          if (lastPot > analogRead(poti) + 5 || lastPot < analogRead(poti) - 5) {
            weckStunde = map(analogRead(poti), 0, 1024, 0, 24);
          }
          weckMen[0] = 'H';
          weckMen[1] = 'H';
          break;
        case 1:
          if (lastPot > analogRead(poti) + 5 || lastPot < analogRead(poti) - 5) {
            weckMinute = map(analogRead(poti), 0, 1024, 0, 60);
          }
          weckMen[0] = 'M';
          weckMen[1] = 'M';
          break;
        case 2:
          if (lastPot > analogRead(poti) + 5 || lastPot < analogRead(poti) - 5) {
            weckTag = map(analogRead(poti), 0, 1024, 0, 11);
          }
          weckMen[0] = 'D';
          weckMen[1] = 'A';
          break;
      }
      lcd.setCursor(0, 3);
      sprintf(lcdChar, "%02d:%02d %03s %02s", weckStunde, weckMinute, wochentag(weckTag), weckMen);
      lcd.print(lcdChar);
      if (okay.checkPressed()) {
        menuID = 3;
        lcd.clear();        
          weckzeiten[weckID].setAlarm(weckStunde, weckMinute, weckTag);
          storage.weckZ[weckID]=weckzeiten[weckID].getInt();
          saveConfig();
          
      }
      break;

  }
}
