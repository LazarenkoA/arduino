
#include "GyverPower.h"
#include <iarduino_RTC.h>


#define lightsensorPIN 3
#define indicatorPIN 13
#define lightPIN 2

const long seconds = 1000;


iarduino_RTC watch(RTC_DS1307);  // для модуля часов
bool previousState = false;

void setup() {
  Serial.begin(9600);
  power.setSleepMode(POWERDOWN_SLEEP);

  watch.begin();  // Инициируем работу с модулем.
  setsystime();

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

  if (millis() % (seconds * 5) == 0) {
    Serial.println(watch.gettime("d-m-Y, H:i:s, D"));
  }

  // мигаем, тип неисправленность
  for (int i = 0; i <= count && !light && light != previousState; i++) {
    digitalWrite(lightPIN, true);
    delay(random(50, 200));
    digitalWrite(lightPIN, false);
    delay(random(50, 200));
  }

  previousState  = !digitalRead(lightsensorPIN);
}


// Установка системного времени
void setsystime() {
  // Ручная установка времени
  //watch.settime(0,0,20,27,2,21,6); // Записываем время в модуль: 0 сек, 51 мин, 21 час, 27, октября, 2015 года, вторник.


  const char* strM = "JanFebMarAprMayJunJulAugSepOctNovDec";  // Определяем массив всех вариантов текстового представления текущего месяца находящегося в предопределенном макросе __DATE__.
  const char* sysT = __TIME__;                                // Получаем время компиляции скетча в формате "SS:MM:HH".
  const char* sysD = __DATE__;                                // Получаем дату  компиляции скетча в формате "MMM:DD:YYYY", где МММ - текстовое представление текущего месяца, например: Jul.
  //  Парсим полученные значения в массив:                    // Определяем массив «i» из 6 элементов типа int, содержащий следующие значения: секунды, минуты, часы, день, месяц и год компиляции скетча.
  const int i[6] {(sysT[6] - 48) * 10 + (sysT[7] - 48), (sysT[3] - 48) * 10 + (sysT[4] - 48), (sysT[0] - 48) * 10 + (sysT[1] - 48), (sysD[4] - 48) * 10 + (sysD[5] - 48), ((int)memmem(strM, 36, sysD, 3) + 3 - (int)&strM[0]) / 3, (sysD[9] - 48) * 10 + (sysD[10] - 48)};

  int dayweek = dayofweek(i[3], i[4], i[5]);
  watch.settime(i[0], i[1], i[2], i[3], i[4], i[5], dayweek);   
}

int dayofweek(int day, int month, int year) {
  // https://lifehacker-ru.turbopages.org/lifehacker.ru/s/kakoj-den-nedeli/

  const int monthCode[12] { 1, 4, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6 };
  int yearCode = (6 + year + int(year / 4)) % 7;
  
  int tmp = ((day + monthCode[month - 1] + yearCode) % 7); // алгоритм возвращает результат так, суббота 0 .... пятница 6, по этому приводим к виду когда воскр. 0 и суббота 6
  
  return (tmp + 6) % 7; // +6 т.к. смещение на 6 позиций
}


// обработчик аппаратного прерывания
void isr() {
  // дёргаем за функцию "проснуться"
  // без неё проснёмся чуть позже (через 0-8 секунд)
  power.wakeUp();
}
