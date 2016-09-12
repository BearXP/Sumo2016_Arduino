

void setup() {
  // put your setup code here, to run once:
  delay(3000);
  pinMode(10,OUTPUT);
  pinMode(13,OUTPUT);
  analogWrite(10,255);
  analogWrite(13,255);
  delay(1000);
  analogWrite(10,0);
  analogWrite(13,0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
