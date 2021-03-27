#define lightsensorPIN 9

void setup() {
  Serial.begin(9600);
  pinMode(lightsensorPIN, INPUT);
}

void loop() {
  Serial.println(digitalRead(lightsensorPIN));  
}
