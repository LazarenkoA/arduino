#include <iarduino_NeoPixel.h>                       //  Подключаем библиотеку iarduino_NeoPixel для работы со светодиодами NeoPixel
iarduino_NeoPixel led(4, 74);                          //  Объявляем объект LED указывая (№ вывода Arduino к которому подключены светодиоды NeoPixel, количество используемых светодиодов)

const long colors[7] = {
  0xFF0000, // красный
  0xde0021,
  0xc90036, 
  0x3000cf,
  0X42AAFF,
  0X00ff00,
  0xdb2400 // фиолетовый
};



void setup() {
  Serial.begin(9600);
  led.begin();
}

void loop() {
  for (int i = 0; i <= sizeof(colors)/sizeof(long)-1; i++) {
    led.setColor(NeoPixelAll, colors[i]);
    led.write();  
 
    delay(1000);
  }
}
