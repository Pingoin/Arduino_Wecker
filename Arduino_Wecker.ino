l//Bibliotheken:
#include <Wire.h> //Für viele I2C: LCD;RTC;
#include <LiquidCrystal_I2C.h> //Fürs LCD
#include <Time.h> //Zeitverwaltung
#include <DS3232RTC.h> //RTC
#include <stdio.h> //für formatierte charArrays
#include <DHT.h> //Feuchte und temperatursensor
#include <Pingoin.h> //Eigene Objektsammlung
#include <EEPROM.h> //speichern im konsistenten speicher
#include <TM1637Display.h> //7Segment display


//Pinnbelegung:
#define poti A2
#define tSensor A0
#define photo A1
#define lcdSpalten 20
#define lcdZeilen 4
#define DHTPIN 7
#define DHTTYPE DHT22
#define klingel 6
#define anzWeck 7
#define lcdStrom 8
#define CLK 9
#define DIO 10
#define CONFIG_VERSION "001"
#define CONFIG_START 64

struct StoreStruct {
  // This is for mere detection if they are your settings
  char version[4];
  int weckZ[7];
  int hell;
  bool sommer;
} storage = {
  CONFIG_VERSION,
  {0, 0, 0, 0, 0, 0, 0},
  100,
  false
};


//Buttons:
PingoButton links(2);
PingoButton rechts(3);
PingoButton menu(4);
PingoButton okay(5);

//laufzeitvariablen:
byte menuID = 0; //Position im Menu 0:=Anzeige
byte subMenuID = 0;
byte secondOffset = 0;
byte secondNew = 0;
tmElements_t tm;
time_t t;
char lcdChar [lcdSpalten]; //String zum Anzeigen, wird für formatierte Ausgaben benötigt
bool wecken = 0;
bool blinken = 0;
AlarmTime weckzeiten[anzWeck];
short int lastPot = 0;
byte weckStunde = 0;
byte weckMinute = 0;
byte weckTag = 0;
byte weckID = 0;
char weckMen[3] = "hh";
unsigned long lastMillis[] = {0}; //Laufzeiten für soft delay
//Blinken,


//Geräte:
LiquidCrystal_I2C lcd(0x27, lcdSpalten, lcdZeilen);//Definition des LCD displays
DHT dht(DHTPIN, DHTTYPE); //Feuchte und Temperatur
TM1637Display display(CLK, DIO);

void setup() {
  //geräte:
  Serial.begin(9600); //Serieller Monitor
  lcd.begin(); //Initialisierung des LCDs
  lcd.clear(); //auf jeden fall mit leerer LCD starten
  setSyncProvider(RTC.get);//Zeit wird regelmäßig mit RTC synchronisiert

  //Pins:
  pinMode(lcdStrom, OUTPUT);
  digitalWrite(13, LOW);
  //Überprüfungen:
  if (timeStatus() != timeSet)
    Serial.println("Konnte nicht mit der RTC verbinden");
  else
    Serial.println("Zeit wurde mit RTC synchronisiert");

  //Einstellungen laden
  loadConfig();
  for (int i = 0; i < 7; i++) {
    weckzeiten[i].setInt(storage.weckZ[i]);
  }
}

void loop() {
  sommerZeit();
  weckentest();
  anzeige();  //Menus anzeigen und Funktionen ausführen
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






