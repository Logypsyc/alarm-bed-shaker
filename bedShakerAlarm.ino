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
  
  if(buttonPressed(12)) {
    if(wakeHour < 23) {
      wakeHour++;
    } 
  }
  if(buttonPressed(11)) {
    if(0 < wakeHour) {
      wakeHour--;
    } 
  }

  if(buttonPressed(10)) {
    if(wakeMinute < 59) {
      wakeMinute++;
    } 
  }
  if(buttonPressed(9)) {
    if(0 < wakeMinute) {
      wakeMinute--;
    } 
  }

  if (buttonPressed(8)) {
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
    if (buttonPressed(7)) {
      digitalWrite(3, LOW);
      delay(randomTime);
      digitalWrite(3, HIGH);
    } 
    
    if (buttonPressed(6)) {
      digitalWrite(3, LOW);
      break;
    }
  }
}


int buttonPressed(uint8_t button) {
  static uint16_t lastStates = 0;
  uint8_t state = digitalRead(button);
  if (state != ((lastStates >> button) & 1)) {
    lastStates ^= 1 << button;
    return state == HIGH;
  }
  return false;
}
