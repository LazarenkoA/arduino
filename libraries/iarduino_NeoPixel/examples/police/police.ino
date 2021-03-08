/*  Скетч демонстрирует один из вариантов применения адресных светодиодов
 *  Номер вывода Arduino и количество используемых светодиодов указывается при объявлении объекта led(вывод,количество)
 */

#include <iarduino_NeoPixel.h>                       //  Подключаем библиотеку iarduino_NeoPixel для работы со светодиодами NeoPixel
iarduino_NeoPixel led(7,4);                          //  Объявляем объект LED указывая (№ вывода Arduino к которому подключены светодиоды NeoPixel, количество используемых светодиодов)

void setup(){
    Serial.begin(9600);                              //  Инициируем передачу данных в монитор последовательного порта
    if(led.begin()) {Serial.println("Ok");}          //  Инициируем работу со светодиодами и выводим сообщение об успешной инициализации
    else            {Serial.println("Err");}         //  Если инициализация невозможна (не хватает памяти для хранения цветов всех светодиодов), то выводим сообщение об ошибке
    led.setColor(NeoPixelAll, 0);                    //  Выключаем все светодиоды (0,RGB)
    led.write();                                     //  Записываем
}

void loop(){
//  ====== МИГАЕМ КРАСНЫМ ЦВЕТОМ ======
    led.setColor(0, 0xFF0000); led.setColor(1, 0xFF0000); led.write(); delay(50);  //  Устанавливаем красный цвет на 0 и 1 светодиод
    led.setColor(NeoPixelAll, 0);                         led.write(); delay(50);  //  Выключаем все светодиоды
    led.setColor(0, 0xFF0000); led.setColor(1, 0xFF0000); led.write(); delay(50);  //  Устанавливаем красный цвет на 0 и 1 светодиод
    led.setColor(NeoPixelAll, 0);                         led.write(); delay(50);  //  Выключаем все светодиоды
    led.setColor(0, 0xFF0000); led.setColor(1, 0xFF0000); led.write(); delay(50);  //  Устанавливаем красный цвет на 0 и 1 светодиод
    led.setColor(NeoPixelAll, 0);                         led.write(); delay(100); //  Выключаем все светодиоды
//  ====== МИГАЕМ СИНИМ ЦВЕТОМ ========
    led.setColor(2, 0x0000FF); led.setColor(3, 0x0000FF); led.write(); delay(50);  //  Устанавливаем синий цвет на 2 и 3 светодиод
    led.setColor(NeoPixelAll, 0);                         led.write(); delay(50);  //  Выключаем все светодиоды
    led.setColor(2, 0x0000FF); led.setColor(3, 0x0000FF); led.write(); delay(50);  //  Устанавливаем синий цвет на 2 и 3 светодиод
    led.setColor(NeoPixelAll, 0);                         led.write(); delay(50);  //  Выключаем все светодиоды
    led.setColor(2, 0x0000FF); led.setColor(3, 0x0000FF); led.write(); delay(50);  //  Устанавливаем синий цвет на 2 и 3 светодиод
    led.setColor(NeoPixelAll, 0);                         led.write(); delay(100); //  Выключаем все светодиоды
}
