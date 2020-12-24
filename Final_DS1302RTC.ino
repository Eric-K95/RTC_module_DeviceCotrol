//Project: RTC DS1302 module
//Sketched by JIN-WOO KIM

#include <stdio.h>
#include <DS1302.h>
#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int kCePin = 5 ; //Chip Enable
const int kIoPin = 6 ; //Input and Output
const int kSclkPin = 7 ; //Serial clock
int ledPin = 13; //Red led light
int buzzerPin = 8; //Buzzer
SimpleTimer timer;


DS1302 rtc(kCePin, kIoPin, kSclkPin);

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time:: kSunday: return "Sunday";
    case Time:: kMonday: return "Monday";
    case Time:: kTuesday: return "Tuesday";
    case Time:: kWednesday: return "Wednesday";
    case Time:: kThursday: return "Thursday";
    case Time:: kFriday: return "Friday";
    case Time:: kSaturday: return "Saturday";
  }
  return "(unkown day)";
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0); //first column, first row
  lcd.print("Hello World!");
  delay(2000);
  lcd.clear();
  rtc.writeProtect(false);
  rtc.halt(false);
  Time t(2020, 12, 15, 14, 17, 0, Time::kThursday);
  rtc.time(t);
  timer.setInterval(30000,beep);
  timer.setInterval(5000,blinkled);
}

void loop() {
  Time t = rtc.time();
  const String day = dayAsString(t.day);
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d", day.c_str(), t.yr, t.mon, t.date, t.hr, t.min, t.sec);
  Serial.println(buf);
  lcd.setCursor(0, 0); //first column, first row
  lcd.print(t.hr); lcd.print(":"); lcd.print(t.min / 10);
  lcd.print(t.min % 10); lcd.print(":"); lcd.print(t.sec / 10); lcd.print(t.sec % 10);
  lcd.setCursor(0, 1); //first column, second row
  lcd.print(t.mon / 10); lcd.print(t.mon % 10); lcd.print("/"); lcd.print(t.date / 10); lcd.print(t.date % 10);
  lcd.print("/"); lcd.print(t.yr / 10); lcd.print(t.yr % 10);
  delay(1000);
  digitalWrite(ledPin,LOW);
  noTone(buzzerPin);
  timer.run();
}

void beep() {
  tone(buzzerPin,500);
  Serial.print("30 seconds over");
  Serial.print("/");
}

void blinkled() {
  digitalWrite(ledPin, HIGH);
  Serial.print("5 seconds over");
  Serial.print("/");
}
