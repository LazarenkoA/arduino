#include <Ultrasonic.h>

#define workPIN 8
#define trigPIN 3
#define echoPIN 2

Ultrasonic ultrasonic(trigPIN, echoPIN);
const long seconds = 1000;
const long minute = seconds * 60;
const long delay_off = minute; // задержка перед выключением

uint32_t myTimer = millis();


void setup()
{
  Serial.begin(9600);

  pinMode(workPIN, OUTPUT);
  digitalWrite(workPIN, HIGH); // выключаем реле
}

void loop()
{
  delay(100);

  int distance = ultrasonic.read();
  bool on = !digitalRead(workPIN);

  if (distance <= 100) {
    Serial.println(distance);
    myTimer = millis();
    digitalWrite(workPIN, LOW);
  }
  if (millis() - myTimer >= delay_off && on) {
    myTimer = millis();
    digitalWrite(workPIN, HIGH);
  }

}
