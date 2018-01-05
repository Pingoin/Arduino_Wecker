//Bibliotheken:
#include <Wire.h> //F�r viele I2C: LCD;RTC;
#include <LiquidCrystal_I2C.h> //Fürs LCD
#include <Time.h> //Zeitverwaltung
#include <TimeLib.h>
#include <Timezone.h> //zeitzone und sommerzeit
#include <DS3232RTC.h> //RTC
#include <stdio.h> //für formatierte charArrays
#include <DHT.h> //Feuchte und temperatursensor
#include <Pingoin.h> //Eigene Objektsammlung
#include <EEPROM.h> //speichern im konsistenten speicher
#include <TM1637Display.h> //7Segment display
#include <Adafruit_BMP085.h>  // Bibliothek für Drucksensoren von Adafruit laden für unseren BMP 180
#include <Encoder.h> //Drehimpulsgeber


//Pinnbelegung/Konstanten:
#define photo A3
#define lcdSpalten 20
#define lcdZeilen 4
#define DHTPIN 11
#define DHTTYPE DHT22
#define klingel 12
#define CLK 10
#define DIO 9
#define CONFIG_VERSION "005"
#define CONFIG_START 64
Encoder myEnc(2, 3);
#define anzWeck 7
struct StoreStruct {
  // This is for mere detection if they aö re your settings
  char version[4];
  int weckZ[7];
  short int hell;
  byte sSegH;

} storage = {
  CONFIG_VERSION,
  {0, 0, 0, 0, 0, 0, 0},
  100,
  0x0F
};


//Buttons:
PingoButton links(5,true);
PingoButton rechts(6,true);
PingoButton menu(7,true);
PingoButton okay(8,true);

//laufzeitvariablen:
byte menuID = 0; //Position im Menu 0:=Anzeige
byte secondOffset = 0;
byte secondNew = 0;
tmElements_t tm;
time_t local, utc;
char lcdChar [lcdSpalten]; //String zum Anzeigen, wird für formatierte Ausgaben benötigt
bool wecken = 0;
bool blinken = 0;
AlarmTime weckzeiten[anzWeck];
short int lastPot = 0;
byte lastHell = 0;
short int lastSwitch = 0;
byte weckStunde = 0;
byte weckMinute = 0;
byte weckTag = 0;
byte weckID = 0;
byte feuchte = 0;
byte tempA = 0;
int druck = 0;
byte tempI = 0;
char weckMen[3] = "hh";
TimeChangeRule myDST = {"MESZ", Last, Sun, Mar, 2, 120};    //Regel für Sommerzeit begin UTC +2
TimeChangeRule mySTD = {"MEZ", Last, Sun, Oct, 3, 60};     //Regel für Standardzeit begin UTC +1
Timezone Zeitzone(myDST, mySTD);//Zeitzone aus Regeln susammenbauen


unsigned long lastMillis[] = {0, 0}; //Laufzeiten für soft delay
//Blinken,Sensoren


//Geräte:
LiquidCrystal_I2C lcd(0x27, lcdSpalten, lcdZeilen);//Definition des LCD displays
DHT dht(DHTPIN, DHTTYPE); //Feuchte und Temperatur
TM1637Display display(CLK, DIO);
Adafruit_BMP085 druckSensor;  // Sensorobjekt mit Namen "druckSensor" erstellen

void setup() {
  //geräte:
  Serial.begin(9600); //Serieller Monitor
  druckSensor.begin();   //zuvor defenierten Sensor "druckSensor" starten
  lcd.init(); //Initialisierung des LCDs
  lcd.backlight();//Hintergrundbeleuchtung an
  lcd.clear(); //auf jeden fall mit leerer LCD starten
  setSyncProvider(RTC.get);//Zeit wird regelmäßig mit RTC synchronisiert

  //Pins:
  digitalWrite(13, LOW);
  //Überprüfungen:
  if (timeStatus() != timeSet)
    Serial.println("Konnte nicht mit der RTC verbinden");
  else
    Serial.println("Zeit wurde mit RTC synchronisiert");

  //Einstellungen laden
  loadConfig();
  for (int i = 0; i < anzWeck; i++) {
    weckzeiten[i].setInt(storage.weckZ[i]);
  }
  display.setColon(true);// Doppelpunkt vom siebensegmenter
}

void loop() {
  weckentest();
  sensoren();
  menu();  //Menus anzeigen und Funktionen ausführen
  klingeln(); //Klingelhandler
}

void klingeln() {
  if (softDelay(0, 500)) {
    if (wecken && blinken) {
      tone(klingel, 666, 500);
    } else {
      if (wecken) blinken = !blinken;
      digitalWrite(klingel, LOW);
    }
  }
}

void sensoren() {
  if (millis() >= lastMillis[1] + 1000) {
    feuchte = int(dht.readHumidity());
    tempA = int(dht.readTemperature());
    druck = int(druckSensor.readPressure() / 100);
    /*tempI;/**/
    Serial.println(feuchte);
    Serial.println(tempA);
    Serial.println(druck);
    lastMillis[1] = millis();
  }
}




