
#define workPIN 8
#define buttonPIN 2
#define indicatorPIN 13

const long seconds = 1000;
const long minute = seconds*60;
const long hour = minute*60;
const long day = hour*24;

boolean included = false;
uint32_t myTimer;

void setup() {
  Serial.begin(9600);
  pinMode(workPIN, OUTPUT);
  pinMode(indicatorPIN, OUTPUT);
  pinMode(buttonPIN, INPUT_PULLUP);

  digitalWrite(workPIN, LOW);
}

void loop() {
  bool on = digitalRead(workPIN);
  bool buttonPressed = !digitalRead(buttonPIN);

  if (millis() - myTimer >= seconds * 30 && on)  { // сколько работаем
    myTimer = millis();

    digitalWrite(workPIN, LOW);
    digitalWrite(indicatorPIN, LOW);
  } else if ((millis() - myTimer >= day * 2 && !on) || (buttonPressed && !on)) { // Сколько отдыхаем
    myTimer = millis();

    digitalWrite(workPIN, HIGH);
    digitalWrite(indicatorPIN, HIGH);
  }


  // отладка
  // delay(500);
  // Serial.println(on);


}
