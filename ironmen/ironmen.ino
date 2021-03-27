
#include "GyverPower.h"

#define lightsensorPIN 3
#define indicatorPIN 13
#define lightPIN 2

const long seconds = 1000;
bool previousState = false;

void setup() {
  Serial.begin(9600);
  power.setSleepMode(POWERDOWN_SLEEP);

  pinMode(lightsensorPIN, INPUT);
  pinMode(lightPIN, OUTPUT);

  // подключаем прерывание на пин
  attachInterrupt(1, isr, FALLING);
}

void loop() {
  //power.sleepDelay(minute);

  bool light = !digitalRead(lightsensorPIN);
  digitalWrite(lightPIN, light);
  int count = random(2, 5);

  // мигаем, тип неисправленность
  for (int i = 0; i <= count && !light && light != previousState; i++) {
    digitalWrite(lightPIN, true);
    delay(random(50, 200));
    digitalWrite(lightPIN, false);
    delay(random(50, 200));
  }

  previousState  = !digitalRead(lightsensorPIN);
}



// обработчик аппаратного прерывания
void isr() {
  // дёргаем за функцию "проснуться"
  // без неё проснёмся чуть позже (через 0-8 секунд)
  power.wakeUp();
}
