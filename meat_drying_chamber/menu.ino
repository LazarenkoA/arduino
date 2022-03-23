// пины энкодера
#define CLK 4 // s2
#define DT 3 // s1
#define SW 2 // key
#define DHT22_PIN 7 // датчик влажности / температуры


#include <EncButton.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHTStable.h"
#include "menuNavigate.h"

EncButton<EB_TICK, DT, CLK, SW> enc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display
DHTStable DHT;

#include "Chart.h"

///// Глобальные переменные //////
uint32_t timerSaveData = 0;
MenuManager* mm;
MenuItem* prev = NULL;
int valueTemp[2];
bool editMode = false;
int humidityData[16];
int temperatureData[16];
const int surveyPeriod = 60 * (60 * 1000); // период через который будут опрашиваться датчики темп. и влажности и сохраняться значение в массив. В минутах.
bool freeze = false;

///////////////////////////////


void setup() {
  Serial.begin(9600);
  lcd.begin(); // initialize the LCD

  // Turn on the blacklight and print a message.
  lcd.backlight();

  mm = new MenuManager();
  mm->Append(new MenuItem("Chart", &sub))->Append(new MenuItem("Fan", &sub))->Append(new MenuItem("Lamp", &sub))->Append(new MenuItem("Temp", &sub));

  mm->Find("Chart");
  mm->AddSub(new MenuItem("", NULL, &chartH, NULL))->Append(new MenuItem("", NULL, &chartT, NULL))->Append(new MenuItem("back", &back, &displayBack, NULL));

  mm->Find("Fan");
  mm->AddSub(new MenuItem("set:", &clickFan))->Append(new MenuItem("back", &back, &displayBack, NULL));

  mm->Find("Lamp");
  mm->AddSub(new MenuItem("set:", &clickLamp))->Append(new MenuItem("back", &back, &displayBack, NULL));

  mm->Find("Temp");
  mm->AddSub(new MenuItem("set:", &clicktemp, &navigateTemp, &settemp))->Append(new MenuItem("back", &back, &displayBack, NULL));

  mm->Find("Chart");

  Show();
  ShowCurrentTemp(true);
}

void loop() {
  enc.tick();
  prev = mm->current;

  if (!editMode) {
    if (enc.right()) {
      mm->Next();
    }
    if (enc.left()) {
      mm->Back();
    }
  }

  if (mm->current != NULL) {
    if (enc.click()) {
      mm->current->Invoke();
    }

    mm->current->SetValue();
  }

  if (mm->current != prev) {
    Show();
  }
  ShowCurrentTemp(false);
  GetData();

  prev = mm->current;
}

void Show() {
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(mm->current->_name);

  if (mm->current->navigate != NULL) {
    mm->current->navigate();
  }
}

// вывод текущей температуры
void ShowCurrentTemp(bool force) {
  if ((millis() % 5000 == 0 || force) && !freeze) { // обновляем раз в 5 секунд
    int chk = DHT.read22(DHT22_PIN);
    if (chk == DHTLIB_OK) {
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("H:" + String(DHT.getHumidity()) + " T:" + String(DHT.getTemperature()));
    }
  }
}

void GetData() {
  if (millis() - timerSaveData >= surveyPeriod || timerSaveData == 0) {
    int chk = DHT.read22(DHT22_PIN);
    if (chk == DHTLIB_OK) {
      timerSaveData = millis();

      push((int*)humidityData, DHT.getHumidity());
      push((int*)temperatureData, DHT.getTemperature());
    }
  }
}

void push(int *data_array, int value) {
  for (int i = 0; i < 16; i++) {
    data_array[i] = data_array[i + 1]; // сдвиг
  }
  data_array[15] = value;
}

int Max(int *data_array) {
  int result = 0;
  for (int i = 0; i < 16; i++) {
    result = max(result, data_array[i]);
  }

  return result;
}

int Min(int *data_array) {
  int result = 10000;
  for (int i = 0; i < 16; i++) {
    result = min(result, data_array[i]);
  }

  return result;

}

int Median(int *data_array) {

}


/////// Обработчики меню /////////

void sub(MenuItem* menu) {
  mm->Sub();
}

void back(MenuItem* menu) {
  mm->ToParent();
  freeze = false;
  ShowCurrentTemp(true);
}

void clickFan(MenuItem* menu) {
  Serial.println("setlamp");
}

void clickLamp(MenuItem* menu) {
  Serial.println("setfan");
}

void clicktemp(MenuItem* menu) {
  menu->tag++;
  int len = sizeof(valueTemp) / sizeof(int);
  editMode = !(menu->tag % (len + 1) == len); // первый клик установка одного значения, второй - второго, третий отключение editMode
  showTemp(menu->tag);
}

void settemp(MenuItem* menu) {
  int len = sizeof(valueTemp) / sizeof(int);

  if (enc.right() && valueTemp[menu->tag % len] >= 0) {
    valueTemp[menu->tag % len]++;
    showTemp(menu->tag);
  }

  if (enc.left() && valueTemp[menu->tag % len] > 0) {
    valueTemp[menu->tag % len]--;
    showTemp(menu->tag);
  }

}

void showTemp(int pos) {
  int len = sizeof(valueTemp) / sizeof(int);

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);

  if (editMode) {
    if (pos % len == 0) {
      lcd.print(mm->current->_name + " " + String(valueTemp[0]) + "*; " + String(valueTemp[1]));
    } else {
      lcd.print(mm->current->_name + " " + String(valueTemp[0]) + "; " + String(valueTemp[1]) + "*");
    }
  } else {
    lcd.print(mm->current->_name + " " + String(valueTemp[0]) + "; " + String(valueTemp[1]));
  }
}

void navigateTemp() {
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(mm->current->_name + " " + String(valueTemp[0]) + "; " + String(valueTemp[1]));
}

void displayBack() {
  byte customChar[2][8] = {
    {0x00, 0x1F, 0x00, 0x04, 0x08, 0x1F, 0x08, 0x04},
    {0x00, 0x1F, 0x01, 0x01, 0x01, 0x1F, 0x00, 0x00},
  };

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.createChar(0, customChar[0]);
  lcd.createChar(1, customChar[1]);
  lcd.setCursor(0, 1);
  lcd.write(0);
  lcd.write(1);
}


void chartH() {
  freeze = true;

  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("H: Max " + String(Max((int*)humidityData)) + ",Min " + String(Min((int*)humidityData)));

  initPlot(); // инициализация символов для отрисовки графика
  drawPlot(0, 1, 16, 1, 0, Max((int*)humidityData), (int*)humidityData);
}

void chartT() {
  freeze = true;

  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("T: Max " + String(Max((int*)temperatureData)) + ",Min " + String(Min((int*)temperatureData)));

  initPlot(); // инициализация символов для отрисовки графика
  drawPlot(0, 1, 16, 1, 0, Max((int*)temperatureData), (int*)temperatureData);
}
