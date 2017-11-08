/*
  Test Algorithm to control LED with a LDR temperature sensor.
*/
int ledPinBuiltin = LED_BUILTIN;
int ledPin = 10;
int ldrPin = A5;

void setup() {
  Serial.begin(115200);
  pinMode(ledPinBuiltin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {

  int state = analogRead(ldrPin);
  Serial.print("Valor lido do LDR: ");
  Serial.println(state);

  int luminosity = map(state,0, 1023, 0, 255);
  Serial.print("Luminosidade: ");
  Serial.println(luminosity);
  Serial.println();

  analogWrite(ledPin, luminosity);

  if (state > 800) {
    digitalWrite(ledPinBuiltin, HIGH);
    analogWrite(ledPin, 1023);
  } else {
    digitalWrite(ledPinBuiltin, LOW);
    if (state < 100)
      analogWrite(ledPin, 0);
  } 
  delay(1000);
}
