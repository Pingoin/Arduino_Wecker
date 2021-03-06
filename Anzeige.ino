void anzeige() {
  bool sSegTest = false;
  if (menuID == 5 && subMenuID == 1) {
    sSegTest = true;
  } else {
    sSegTest = false;
  }
  if (analogRead(photo) > storage.hell && !sSegTest) {
    display.setBrightness(0x00);//helligkeit auf maximum
    lcd.backlight();
  } else if (!sSegTest) {
    display.setBrightness(storage.sSegH);//helligkeit auf maximum
    lcd.backlight();
  } else {
    display.setBrightness(storage.sSegH);//helligkeit auf maximum
    lcd.noBacklight();
  }

  lcd.setCursor(0, 0);
  sprintf(lcdChar, "%2s;%2s;%2s;%2s;%2s;%2s;%2s", weckzeiten[0].getString(), weckzeiten[1].getString(), weckzeiten[2].getString(), weckzeiten[3].getString(), weckzeiten[4].getString(), weckzeiten[5].getString(), weckzeiten[6].getString());
  lcd.print(lcdChar);/**/
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
      lcd.setCursor(0, 1);/**/
      /*lcd.setCursor(0, 0); /**/
      printTime();
      lcd.setCursor(0, 2);/**/
      /*lcd.setCursor(0, 1); /**/
      subMenuMax(3);
      switch (subMenuID) {
        case 0:
          sprintf(lcdChar, "%3d %%;%2d%cC;%4d hPa         ", feuchte, tempA, char(223),druck);
          lcd.print(lcdChar);
          break;
        case 1:
          lcd.print("Uhr einstellen      ");
          if (okay.checkPressed()) {
            menuID = 1;
            subMenuID = 0;
            tm = getLocalTime();
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
        case 3:
          lcd.print("Einstellungen       ");
          if (okay.checkPressed()) {
            menuID = 5;
            weckID = 0;
            lastPot = analogRead(poti);
            lcd.clear();
            lastSwitch = storage.hell;
            lastHell = storage.sSegH;
          }
          break;
          /*16:2lcd
            case 4:
            sprintf(lcdChar, "%2s;%2s;%2s;%2s;%2s;%2s;%2s", weckzeiten[0].getString(), weckzeiten[1].getString(), weckzeiten[2].getString(), weckzeiten[3].getString(), weckzeiten[4].getString(), weckzeiten[5].getString(), weckzeiten[6].getString());
            lcd.print(lcdChar);
            break;
          */

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
      printTime();/**/
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
        saveConfig();
      }
      subMenuMax(6);

      /*
        lcd.setCursor(0, 0);
        sprintf(lcdChar, "%2s;%2s;%2s;%2s;%2s;%2s;%2s", weckzeiten[0].getString(), weckzeiten[1].getString(), weckzeiten[2].getString(), weckzeiten[3].getString(), weckzeiten[4].getString(), weckzeiten[5].getString(), weckzeiten[6].getString());
        lcd.print(lcdChar);
      */
      lcd.setCursor(subMenuID * 3, 1);
      lcd.print("++");
      lcd.setCursor(0, 2);
      printTime();/**/

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
      printTime();/**/
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
            weckTag = map(analogRead(poti), 0, 1024, 0, weckzeiten[weckID].getDays());
          }
          weckMen[0] = 'D';
          weckMen[1] = 'A';
          break;
      }
      lcd.setCursor(0, 3);
      sprintf(lcdChar, "%02d:%02d %03s %02s", weckStunde, weckMinute, weckzeiten[weckID].createStr(weckTag), weckMen);
      lcd.print(lcdChar);
      if (okay.checkPressed()) {
        menuID = 3;
        lcd.clear();
        weckzeiten[weckID].setAlarm(weckStunde, weckMinute, weckTag);
        storage.weckZ[weckID] = weckzeiten[weckID].getInt();
        saveConfig();
      }
      break;
    case 5:  /*+++++++++++Einstellungen+++++++++*/
      lcd.setCursor(0, 1);
      printTime();
      lcd.setCursor(0, 2);
      if (rechts.checkPressed()) {
        subMenuID++;
        storage.sSegH = lastHell;
        storage.hell = lastSwitch;
        lcd.clear();
      }
      if (links.checkPressed()) {
        subMenuID--;
        storage.sSegH = lastHell;
        storage.hell = lastSwitch;
        lcd.clear();
      }
      subMenuMax(1);
      switch (subMenuID) {
        case 0:
          lcd.print("Schalthelligkeit");
          lcd.setCursor(0, 3);
          sprintf(lcdChar, "%04d--%04d/%04d", lastSwitch, storage.hell, analogRead(photo));
          lcd.print(lcdChar);
          if (menu.checkPressed()) {
            menuID = 0;
            storage.hell = lastSwitch;
            lcd.clear();
          }
          storage.hell = analogRead(poti);
          if (okay.checkPressed()) {
            menuID = 0;
            saveConfig();
            lcd.clear();
          }
          break;
        case 1:
          lcd.print("Helligkeit");
          lcd.setCursor(0, 3);
          sprintf(lcdChar, "%04d--%04d", lastHell, storage.sSegH);
          lcd.print(lcdChar);
          if (menu.checkPressed()) {
            menuID = 0;
            storage.sSegH = lastHell;
            lcd.clear();
          }
          storage.sSegH = map(analogRead(poti), 0, 1023, 0x00, 0x0F);
          if (okay.checkPressed()) {
            menuID = 0;
            saveConfig();
            lcd.clear();
          }
          break;
      }


      break;

  }
}
