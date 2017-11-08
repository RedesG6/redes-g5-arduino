/*
  Ambiental Monitor using luminosity and temperature sensors.

  TO DO:
  - Communicating with a rPI through serial.

  Opcionals:
  - Convert ldr reading into Lux (necessary to make a chart table with lux readings)
*/
#define referencePotencial 5.0 // Power source to LDR
#define miliToVolts 100.0   // convert mV to V (Volts)
#define timeInterval 1.0 // Time interval in minutes - minimum 1.0/60.0, due reading time
#define millisToMinute 1000*60

int ledPinBuiltin = LED_BUILTIN;
int ledPin = 10;
int ldrPin = A5;
int lmPin = A0;

void setup() {
  Serial.begin(115200);
  Serial.println("Hora inicial: 10:30");
  pinMode(ledPinBuiltin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {

  int ldrReading = 0;
  int luminosity = 0;
  int celsiusTemp = 0;
  int fahrenheitTemp = 0;

  logReadingTime();

  temperatureReading(celsiusTemp, fahrenheitTemp);
  temperatureLogging(celsiusTemp, fahrenheitTemp);

  luminosityReading(ldrReading, luminosity);
  luminosityLogging(ldrReading, luminosity);

  delay(calculateDelayTime());
}

void logReadingTime() {
  Serial.print("T+ ");

  long segundos = millis() / 1000;
  long minutos = segundos / 60;
  segundos -= minutos * 60;
  long horas = minutos / 60;
  minutos -= horas * 60;

  Serial.print(horas);
  Serial.print(":");
  Serial.print(minutos);
  Serial.print(":");
  Serial.print(segundos);
  Serial.println(" HH:mm:ss");
}

void luminosityReading(int& state, int& luminosity) {
  state = analogRead(ldrPin);
  Serial.print("Valor lido do LDR: ");
  Serial.println(state);

  luminosity = map(state, 0, 1023, 0, 255);
  Serial.print("Luminosidade: ");
  Serial.println(luminosity);
  Serial.println();
}

void luminosityLogging(int& state, int& luminosity) {
  analogWrite(ledPin, luminosity);

  if (state > 800) {
    digitalWrite(ledPinBuiltin, HIGH);
    analogWrite(ledPin, 1023);
  } else {
    digitalWrite(ledPinBuiltin, LOW);
    if (state < 100)
      analogWrite(ledPin, 0);
  }
}

void temperatureReading(int& celsius, int& farenheit) {
  int samples[8];

  for (int i = 0; i <= 7; i++) {
    samples[i] = ( referencePotencial * analogRead(lmPin) * miliToVolts) / 1024.0;
    //A cada leitura, incrementa o valor da variavel celsiusTemp
    celsius = celsius + samples[i];
    delay(100);
  }
  celsius = celsius / 8.0;
  farenheit = (celsius * 9) / 5 + 32;
}

void temperatureLogging(int& celsius, int& farenheit) {
  Serial.print("Temperatura: ");
  Serial.print(celsius, DEC);
  Serial.print(" \u00B0C, ");
  Serial.print(farenheit, DEC);
  Serial.println(" \u00B0F.");
  Serial.println();
}

long calculateDelayTime() {
  return (long) (timeInterval * millisToMinute) - millis() % (long)(timeInterval * millisToMinute);
}

