
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
long pause = hour; // Сколько отдыхаем
uint32_t tmp =  myTimer + pause; // первичная инициализация
String txt;

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
  
  if (tmp >= millis()) {
    txt = "Сл. полив через " + format(tmp - millis());
  }

  if (millis() - myTimer >=  work && on)  { 
    myTimer = millis();
    tmp = myTimer + pause;
    
    digitalWrite(workPIN, LOW);
    digitalWrite(indicatorPIN, LOW);
  } else if ((millis() - myTimer >=  pause && !on) || (buttonPressed && !on)) { 
    myTimer = millis();
    tmp = 0;
    txt = "Поливаем...";
    
    digitalWrite(workPIN, HIGH);
    digitalWrite(indicatorPIN, HIGH);
  }


  // отладка
   delay(1000);
   Serial.println(txt); 
   
}


String format(long i) {
  int h = i / 3600000; // 3600000 - миллисекунд в часе
  int m = ((i / 1000) % 3600) / 60;
  int s = (i / 1000) % 60;

  return inttostr(h) + ":" + inttostr(m) + ":"+ inttostr(s); 
}

String inttostr(int i) {
  String result;
  
   if (i < 10) result += "0";
  result += String(i, DEC);

  return result;
}
