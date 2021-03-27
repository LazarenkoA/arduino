#include <iarduino_NeoPixel.h>  //  Подключаем библиотеку iarduino_NeoPixel для работы со светодиодами NeoPixel
#include <iarduino_RTC.h>

#define LED_PIN 4
#define LED_count 74
#define lightsensorPIN 9
#define RezistorPIN A6
#define maxRezistor 1023.

iarduino_NeoPixel led(LED_PIN, LED_count);  //  Объявляем объект LED указывая (№ вывода Arduino к которому подключены светодиоды NeoPixel, количество используемых светодиодов)
iarduino_RTC watch(RTC_DS3231);  // для модуля часов

class sunriseSunset {
  public:
    sunriseSunset(byte sunrise, byte sunset);
    byte _sunrise;
    byte _sunset;
};

sunriseSunset::sunriseSunset(byte sunrise, byte sunset) {
  _sunrise = sunrise;
  _sunset = sunset;
}


const long colors[7] = {
  0xff3200, // фиолетовый
  0xFF0000, // красный
  0xde0021,
  0xff00ff,
  0x3000cf,
  0X00ff88,
  0X00ff00 // синий
};

const char*  holidays[2] {
  "2908", // 29.08 = 2908
  "3112"  // 31.12 = 3112
  "0101"  // 01.01
};

uint8_t j;                                     //  Объявляем переменную для хранения значения сдвига спектра цветов для всех светодиодов (от 0 до 255)
uint8_t k;                                     //  Объявляем переменную для хранения положения сдвига спектра цвета для каждого светодиода на спектре j (зависит от количества светодиодов)
uint8_t r, g, b;                               //  Объявляем переменную для хранения цветов RGB для каждого светодиода
const uint8_t z = 5;                           //  Определяем константу указывающую задержку в мс (чем выше значение, тем медленнее перелив цветов)


void setup() {
  Serial.begin(9600);
  led.begin(); // Инициируем работу с лентой
  watch.begin();  // Инициируем работу с модулем времени

  pinMode(lightsensorPIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {

  //Serial.println(watch.gettime("d-m-Y, H:i:s, D"));

  // Если в Serial послали resettime, тогда устанавливаем время
  String str = Serial.readString();
  str.trim();
  str.toLowerCase();
  if (str == "resettime") {
    setsystime();
    Serial.print("Время установлено ");
    Serial.println(watch.gettime("d-m-Y, H:i:s, D"));
  }

  //if (!digitalRead(lightsensorPIN)) {
  if (!enable()) {
    led.setColor(NeoPixelAll, 0x000000);
    led.write();
    delay(500);
    return;
  }

  if (isHoliday()) {
    rainbow();
  } else {
    led.setColor(NeoPixelAll, colors[watch.weekday], analogRead(A6) / maxRezistor * 100); // яркость успавляется переменным резистором
    led.write();
    delay(500);
  }
}


bool isHoliday() {
  for (int i = 0; i <= sizeof(holidays) / sizeof(int) - 1; i++) {
    if (watch.gettime("dm") == holidays[i]) {
      return true;
    }
  }
  return false;
}


bool enable() {
  // массив данных восхода - заката
  sunriseSunset obj[12] {
    sunriseSunset(8, 17),
    sunriseSunset(7, 18),
    sunriseSunset(6, 19),
    sunriseSunset(6, 20),
    sunriseSunset(5, 20),
    sunriseSunset(5, 21),
    sunriseSunset(5, 22),
    sunriseSunset(5, 21),
    sunriseSunset(5, 20),
    sunriseSunset(6, 20),
    sunriseSunset(6, 19),
    sunriseSunset(7, 18)
  };


  watch.gettime();
  return !(watch.Hours >= obj[watch.month-1]._sunrise and watch.Hours <= obj[watch.month-1]._sunset);
}


void rainbow() {
  j++;                                             //  Смещаем спектр цветов для всех светодиодов
  for (uint16_t i = 0; i < led.count(); i++) {     //  Проходим по всем светодиодам
    k = ((uint16_t)(i * 256 / led.count()) + j); //  Определяем положение очередного светодиода на смещённом спектре цветов
    if (k < 85) {
      b = 0;
      r = k * 3;
      g = 255 - r;
    } else  //  Перелив от зелёного к красному, через жёлтый
      if (k < 170) {
        k -= 85;
        g = 0;
        b = k * 3;
        r = 255 - b;
      } else  //  Перелив от красного к синему  , через фиолетовый
      {
        k -= 170;  //  Перелив от синего   к зелёному, через голубой
        r = 0;
        g = k * 3;
        b = 255 - g;
      }
    led.setColor(i, r, g, b);                    //  Устанавливаем выбранный цвет для очередного светодиода
  }

  led.write();                                 //  Записываем цвета всех светодиодов
  delay(z);                                        //  Устанавливаем задержку
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
