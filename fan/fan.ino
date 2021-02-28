#define workPIN 4
#define lightsensorPIN 2

const long seconds = 1000;
const long delay_on = seconds * 30; // задержка перед включением
const long delay_off = seconds * 60*5; // задержка перед выключением


void setup() {
  //Serial.begin(9600);
  
  pinMode(workPIN, OUTPUT);
  pinMode(lightsensorPIN, INPUT);
}

void loop() {
  bool light = !digitalRead(lightsensorPIN);
  bool on = digitalRead(workPIN);

  // если включили свет начинаем отсчет времени, потом вкллючаем вентилятор
  if (light && !on) {
    delay(delay_on);
    digitalWrite(workPIN, !digitalRead(lightsensorPIN)); // к этому моменту усет уже может быть выключен, по этому читаем еще раз
  } else if (!light && on) {
    delay(delay_off);
    digitalWrite(workPIN, !digitalRead(lightsensorPIN));
  }

  // не точный алгоритм
  // uint32_t m = millis();
//  if (light && m > 0 && m % delay_on == 0 && !on) {
//    digitalWrite(workPIN, HIGH);
//  } else if (!light && m > 0 && m % delay_off == 0 && on) {
//    digitalWrite(workPIN, LOW);
//  }

}
