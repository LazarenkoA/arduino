
#define workPIN 8
#define buttonPIN 2
#define indicatorPIN 13

const long seconds = 1000;
const long minute = seconds*60;
const long hour = minute*60;
const long day = hour*24;

boolean included = false;
uint32_t myTimer = millis();
long work = seconds * 30; // сколько работаем
long pause = seconds * 30; // Сколько отдыхаем
uint32_t tmp =  myTimer + pause; // первичная инициализация


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
  uint32_t m = millis();

  if (m - myTimer >=  work && on)  { 
    myTimer = m;
    tmp = myTimer + pause;
    
    digitalWrite(workPIN, LOW);
    digitalWrite(indicatorPIN, LOW);
  } else if ((m - myTimer >=  pause && !on) || (buttonPressed && !on)) { 
    myTimer = m;
    tmp = 0;

    digitalWrite(workPIN, HIGH);
    digitalWrite(indicatorPIN, HIGH);
  }


  // отладка
   delay(1000);
   if (tmp > 0 && tmp > m) {
    Serial.println(format(tmp - m)); 
   }
   

}


String format(long i) {
  int h = i / 3600000; // 3600000 - миллисекунд в часе
  int m = ((i / 1000) % 3600) / 60;
  int s = (i / 1000) % 60;

  return String(h, DEC) + ":" + String(m, DEC) + ":" + String(s, DEC);
}
