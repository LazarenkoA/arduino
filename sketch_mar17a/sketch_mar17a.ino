#include <iarduino_NeoPixel.h>  //  Подключаем библиотеку iarduino_NeoPixel для работы со светодиодами NeoPixel

#define LED_PIN 4
#define lightsensorPIN 3
#define mosfet_PIN 7
#define LED_count 74

iarduino_NeoPixel led(LED_PIN, LED_count);  //  Объявляем объект LED указывая (№ вывода Arduino к которому подключены светодиоды NeoPixel, количество используемых светодиодов)

void setup() {
  Serial.begin(9600);

  led.begin(); // Инициируем работу с лентой
  led.setColor(NeoPixelAll, 0xFF0000);
  led.write();

  pinMode(mosfet_PIN, OUTPUT);
  pinMode(lightsensorPIN, INPUT);
  digitalWrite(mosfet_PIN, HIGH);
}

void loop() {

strip.setPixelColor(i, strip.Color(0, 0, 0));

//  delay(1000);
//  digitalWrite(mosfet_PIN, HIGH);
//  Serial.println(digitalRead(mosfet_PIN));
//  delay(1000);
//  digitalWrite(mosfet_PIN, LOW);
//  Serial.println(digitalRead(mosfet_PIN));
}
