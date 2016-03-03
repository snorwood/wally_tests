const int ledPin =  13;

void setup() {
  pinMode(40, INPUT);
  pinMode(36, OUTPUT);
  digitalWrite(36, HIGH);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, LOW);
  while(!digitalRead(40));
  digitalWrite(13, HIGH);
  while(digitalRead(40));
}
