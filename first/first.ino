void setup() {
  Serial.begin(9600);
  Serial.print("жопа");

  pinMode(13, INPUT);
}  

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(13, 1);

  delay(500);
  digitalWrite(13, 0);
  delay(500);
}
