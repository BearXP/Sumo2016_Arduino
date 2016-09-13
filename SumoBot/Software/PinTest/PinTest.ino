#define analog
#define pin 9

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("255");
  digitalWrite(pin, HIGH);
  delay(2000);
  Serial.println("0");
  digitalWrite(pin, LOW);
  delay(2000);
}
