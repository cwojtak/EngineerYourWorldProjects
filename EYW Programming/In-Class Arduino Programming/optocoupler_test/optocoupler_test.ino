//Connor Wojtak
//optocoupler_test.ino

void setup() {
  // put your setup code here, to run once:
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  delay(5000);
  digitalWrite(8, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);
  digitalWrite(8, HIGH);
  delay(500);
  digitalWrite(8, LOW);
}
