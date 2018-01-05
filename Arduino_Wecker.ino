#include <AlarmTime.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h> //für formatierte charArrays
#include <I2Cdev.h>
#include <TM1637Display.h>
#include <DHT.h>

//Pinbelegungen
#define klingel 3
#define aus 2
#define kick 4
#define CLK 6
#define DIO 5
#define DHTPIN 2 
#define DHTTYPE DHT22

//Laufzeitvariablen
bool wecken = 0;
bool blinken = 0;
const int anzWeck = 5;
AlarmTime weckzeiten[anzWeck];
char uhrzeit [16];
char wetter [16];



//Geräte
LiquidCrystal_I2C lcd(0x27, 16, 2);
TM1637Display display(CLK, DIO);
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  //Geräte
  Serial.begin(9600);
  Wire.begin();
  lcd.begin();
  setSyncProvider(RTC.get);
  dht.begin();

  //Pins    
  pinMode(klingel, OUTPUT);
  pinMode(aus, INPUT);
  pinMode(kick, INPUT);


  //Überprüfung
  Serial.println("IC2 Geräte prüfen");
  if (timeStatus() != timeSet)
    Serial.println("Keine Zeitsyncro mit RTC");
  else
    Serial.println("Zeit erfolgreich mit RTC syncronisiert");

  weckzeiten[1].setAlarm(7,30,0,1);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (second() <= 2) {
    for (int i = 0; i < anzWeck; i++) {
      if (weckzeiten[i].getAlarm(hour(),minute(),weekday())) {
        wecken = 1;
      }
    }
  }

  if (digitalRead(kick) == HIGH) {
    wecken = 1;
  }
  if (digitalRead(aus) == HIGH) {
    wecken = 0;
  }


  if (second()==0||second()==15||second()==30||second()==45){
    lcd.setCursor(0,0);
    sprintf(wetter,"%3d %%; %2d%cC",int(dht.readHumidity()),int(dht.readTemperature()),char(223)); //char(223) grad zeichen direkt versteht er nicht
    lcd.print(wetter);
  }
  lcd.setCursor(0,1);
  sprintf(uhrzeit,"    %02d:%02d:%02d    ",hour(),minute(),second());
  lcd.print(uhrzeit);
  display.setBrightness(0x0f);//helligkeit auf maximum
  display.showNumberDec(hour()*100+minute(), true);

  if (wecken) {
    if (blinken) {
      tone(klingel,666,500);
    } 
    else {
        digitalWrite(klingel, LOW);
      delay(500);
    }
    blinken = !blinken;
  } 
  else {
    digitalWrite(klingel, LOW);
    delay(5);
  }
}

