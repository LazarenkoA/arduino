#include <iarduino_NeoPixel.h>  //  Подключаем библиотеку iarduino_NeoPixel для работы со светодиодами NeoPixel

#define LED_PIN 6
#define LED_count 1

iarduino_NeoPixel led(LED_PIN, LED_count);  //  Объявляем объект LED указывая (№ вывода Arduino к которому подключены светодиоды NeoPixel, количество используемых светодиодов)

int val;
int newVal;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

  led.begin(); // Инициируем работу с лентой
}

void rgb (int red, int green, int blue) {
  led.setColor(NeoPixelAll, red, green, blue); // яркость успавляется переменным резистором
  Serial.println("R:"+ String(red, DEC) + " G:"+ String(green, DEC) + " B:" + String(blue, DEC));
  led.write();
}

void loop() {

  // put your main code here, to run repeatedly:
  val = analogRead(A5);
  newVal = map(val, 0, 1023, 0, 1530);

  if (newVal <= 255) { //красный
    rgb(newVal, 0, 0);
  } else if ((newVal > 255) && (newVal <= 510)) { //оранжевый и желтый
    rgb(255, newVal - 255, 0);
  } else if ((newVal > 510) && (newVal <= 765)) { //зеленый
    rgb(765 - newVal, 255, 0);
  } else if ((newVal > 765) && (newVal <= 1020)) { //голубой
    rgb(0, 255, newVal - 765);
  } else if ((newVal > 1020) && (newVal <= 1275)) { //синий
    rgb(0, 1275 - newVal, 255);
  } else if ((newVal > 1275) && (newVal <= 1530)) { //фиолетовый
    rgb(newVal - 1275, 0, 255);
  }


  delay (100);
}
