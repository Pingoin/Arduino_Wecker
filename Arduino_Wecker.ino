#include <AlarmTime.h>
#include <DS3232RTC.h>
#include <Time.h>
#include <Wire.h>

//Pinbelegungen
int klingel = 3;
int aus = 2;
int kick = 42;

//Laufzeitvariablen
bool wecken = 0;
bool blinken = 0;
const int anzWeck = 5;
AlarmTime weckzeiten[anzWeck];




void setup() {
  // put your setup code here, to run once:
  pinMode(klingel, OUTPUT);
  pinMode(aus, INPUT);
  pinMode(kick, INPUT);
  Serial.begin(9600);
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");
  weckzeiten[0].setAlarm(11,55,9,1);

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalClockDisplay();
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
  if (wecken) {
    if (blinken) {
      tone(klingel,666,500)
    } else {
      digitalWrite(klingel, LOW);
			delay(500);
    }
    blinken = !blinken;
  } else {
    digitalWrite(klingel, LOW);
		delay(500);
  }
}

void digitalClockDisplay(void)
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(' ');
  Serial.print(weekday());
  Serial.print(' ');
  Serial.print(day());
  Serial.print(' ');
  Serial.print(month());
  Serial.print(' ');
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(':');
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
