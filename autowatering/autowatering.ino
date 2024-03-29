#include <avr/eeprom.h>
#include <U8glib.h>

// шрифты
#include "rus9x18.h" 
#include "rus8x13.h"

#define workPIN 4
#define buttonPIN 2
#define indicatorPIN 13

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

const long seconds = 1000;
const long minute = seconds * 60;
const long hour = minute * 60;
const long day = hour * 24;
const long timeout = seconds * 4; // таймаут того сколько нужно держать кнопку что бы задать время полива

boolean included = false;
uint32_t myTimer = millis();
uint32_t buttonTimer;
long work = seconds * 10; // сколько работаем (по дефолту, можно изменить удержанием кнопки)
long pause = day * 4; // Сколько отдыхаем
uint32_t tmp =  myTimer + pause; // первичная инициализация
String txt;
bool previousState = false; // для определения нажатия и удержания кнопки
int startpos = 5;

void setup() {
  Serial.begin(9600);

  pinMode(workPIN, OUTPUT);
  pinMode(workPIN, INPUT);
  pinMode(indicatorPIN, OUTPUT);
  pinMode(buttonPIN, INPUT_PULLUP);

  digitalWrite(workPIN, LOW);

  long tmp = eeprom_read_dword(0); // читаем из энергонезависимой памяти
  if (tmp > 0) {
    work = tmp;
  }
}

void loop() {
  bool on = digitalRead(workPIN);
  bool buttonPressed = !digitalRead(buttonPIN);
  bool forcebreak = false;

  // определяем удержание кнопки
  if (buttonPressed && buttonTimer == 0) {
    buttonTimer = millis();
  }

  // если кнопку держим определенное время то отсчитываем время полива
  if (buttonTimer > 0 && millis() - buttonTimer >= timeout) {
    Serial.println("Установка времени полива");

    myTimer = millis(); // меняем что бы таймер не закончился, полив прервется через forcebreak
    forcebreak = buttonPressed != previousState && !buttonPressed; // пренудительно останавливаем полив когда кнопку отпустили
    if (forcebreak) {
      work = millis() - buttonTimer;
      eeprom_write_dword(0, work); // запись в энергонезависимую запись (что б значение осталось при выключении питания)
    }
  }

  // когда отпустили кнопку, прерываем отсчет времени
  if (buttonPressed != previousState && !buttonPressed) {
    buttonTimer = 0;
  }


  if (tmp >= millis()) {
    txt = "Сл. полив через " + format(tmp - millis());
  }

  if (millis() - myTimer >= work && on || forcebreak)  {
    myTimer = millis();
    tmp = myTimer + pause;
    buttonTimer = 0;

    digitalWrite(workPIN, LOW);
    digitalWrite(indicatorPIN, LOW);
  } else if ((millis() - myTimer >= pause && !on) || (buttonPressed && !on)) {
    myTimer = millis();
    tmp = 0;
    txt = "Поливаем...";

    digitalWrite(workPIN, HIGH);
    digitalWrite(indicatorPIN, HIGH);
  }

  previousState = !digitalRead(buttonPIN);


  // Отрисовка дисплея
  u8g.firstPage();
  int displaylength = 50;
  do
  {
    u8g.setColorIndex(1);    // цвет белый
    u8g.setFont(rus9x18);
    u8g.setPrintPos(startpos, 15);
    u8g.print(txt);

    u8g.setFont(rus8x13);
    u8g.setPrintPos(5, 30);
    u8g.print("полив " + String(work / seconds , DEC) + " сек");

  } while ( u8g.nextPage() );

  startpos -= 10;
  if (startpos < -150) {
    startpos = 5;
  }
  //Serial.println(startpos);
  delay(500);
}


String format(long i) {
  int d = i / 1000 / 86400; // 86400 - сек. в сутках
  int h = ((i / 1000) % 86400) / 3600;
  int m = ((i / 1000) % 3600) / 60;
  int s = (i / 1000) % 60;

  char buff[10];

  // если больше дня, то выводим дни, если менише, то часы
  if (d > 0) {
    return inttostr(d) + " дня " + inttostr(h) + " ч." ;
  }

  return inttostr(h) + ":" + inttostr(m) + ":" + inttostr(s);
}

String inttostr(int i) {
  String result;

  if (i < 10) result += "0";
  result += String(i, DEC);

  return result;
}
