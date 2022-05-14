#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <EEPROM.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int wakeHour = EEPROM.read(0);
int wakeMinute = EEPROM.read(1);

uint32_t syncProvider()
{
  return rtc.now().unixtime();
}

void setup() {
  rtc.begin();
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = rtc.now();
  setTime(now.hour(), now.minute(), now.second(), now.month(), now.day(), now.year());
  pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  Alarm.alarmRepeat(wakeHour, wakeMinute, 0, wakeAlarm);
}

void loop() {
  DateTime now = rtc.now();
  Alarm.delay(1000); 
  lcd.setCursor(0,0);
  lcd.print("Time: ");
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) {
    lcd.print("0");
    lcd.print(now.minute());
  }
  else {
    lcd.print(now.minute());
  }
  lcd.print(":");
  if (now.second() < 10) {
    lcd.print("0");
    lcd.print(now.second());
  }
  else {
    lcd.print(now.second());  
  }
  lcd.setCursor(0, 1);
  lcd.print("Wake Time: ");
  if (wakeHour < 10) {
    lcd.print(" ");
    lcd.print(wakeHour);
  }
  else {
    lcd.print(wakeHour);    
  }
  lcd.print(":");
  if (wakeMinute < 10) {
    lcd.print("0");
    lcd.print(wakeMinute);
  }
  else {  
    lcd.print(wakeMinute);
  }
  
  if(buttonPressedHour(12)) {
    if(wakeHour < 23) {
      wakeHour++;
    } 
  }
  if(buttonPressedHour(11)) {
    if(0 < wakeHour) {
      wakeHour--;
    } 
  }

  if(buttonPressedMinute(10)) {
    if(wakeMinute < 59) {
      wakeMinute++;
    } 
  }
  if(buttonPressedMinute(9)) {
    if(0 < wakeMinute) {
      wakeMinute--;
    } 
  }

  if (buttonPressedConfirm(8)) {
    EEPROM.update(0, wakeHour);
    EEPROM.update(1, wakeMinute);
    setup();
    digitalWrite(13, HIGH);   
    delay(100);              
    digitalWrite(13, LOW);   
    
  }
}

void wakeAlarm() {
  const int stopAlarmButton = digitalRead(6);
  const int snoozeAlarmButton = digitalRead(7);
  while (true) {
    digitalWrite(3, HIGH);
    int randomNumber = rand() % 11;
    int randomTime = randomNumber * 1000;
    if (buttonPressedSnooze(7)) {
      digitalWrite(3, LOW);
      delay(randomTime);
      digitalWrite(3, HIGH);
    } 
    
    if (buttonPressedStop(6)) {
      digitalWrite(3, LOW);
      break;
    }
  }
}


int buttonPressedHour(uint8_t buttonHour) {
  static uint16_t lastStatesHour = 0;
  uint8_t stateHour = digitalRead(buttonHour);
  if (stateHour != ((lastStatesHour >> buttonHour) & 1)) {
    lastStatesHour ^= 1 << buttonHour;
    return stateHour == HIGH;
  }
  return false;
}

int buttonPressedMinute(uint8_t buttonMinute) {
  static uint16_t lastStatesMinute = 0;
  uint8_t stateMinute = digitalRead(buttonMinute);
  if (stateMinute != ((lastStatesMinute >> buttonMinute) & 1)) {
    lastStatesMinute ^= 1 << buttonMinute;
    return stateMinute == HIGH;
  }
  return false;
}

int buttonPressedSnooze(uint8_t buttonSnooze) {
  static uint16_t lastStatesSnooze = 0;
  uint8_t stateSnooze = digitalRead(buttonSnooze);
  if (stateSnooze != ((lastStatesSnooze >> buttonSnooze) & 1)) {
    lastStatesSnooze ^= 1 << buttonSnooze;
    return stateSnooze == HIGH;
  }
  return false;
}

int buttonPressedStop(uint8_t buttonStop) {
  static uint16_t lastStatesStop = 0;
  uint8_t stateStop = digitalRead(buttonStop);
  if (stateStop != ((lastStatesStop >> buttonStop) & 1)) {
    lastStatesStop ^= 1 << buttonStop;
    return stateStop == HIGH;
  }
  return false;
}

int buttonPressedConfirm(uint8_t buttonConfirm) {
  static uint16_t lastStatesConfirm = 0;
  uint8_t stateConfirm = digitalRead(buttonConfirm);
  if (stateConfirm != ((lastStatesConfirm >> buttonConfirm) & 1)) {
    lastStatesConfirm ^= 1 << buttonConfirm;
    return stateConfirm == HIGH;
  }
  return false;
}
