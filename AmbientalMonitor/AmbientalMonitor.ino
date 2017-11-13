/*
  Ambiental Monitor using luminosity and temperature sensors.

  TO DO:
  - Communicating with a rPI through serial.

  Opcionals:
  - Convert ldr reading into Lux (necessary to make a chart table with lux readings)
*/

#include <ArduinoJson.h>

#define referencePotencial 5.0 // Power source to LDR
#define miliToVolts 100.0   // convert mV to V (Volts)
#define timeInterval 1.0/12.0 // Time interval in minutes - minimum 1.0/60.0, due reading time
#define millisToMinute 1000*60

int ledPinBuiltin = LED_BUILTIN;
int ledPin = 10;
int ldrPin = A5;
int lmPin = A0;

void setup() {
  Serial.begin(9600);
  while (!Serial) ;

  // Para desenvolvimento/controle
//  Serial.println("Hora inicial: 10:30");
  
  pinMode(ledPinBuiltin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPinBuiltin, HIGH);
}

void loop() {
  if (digitalRead(ledPinBuiltin == HIGH))
    digitalWrite(ledPinBuiltin, LOW);
  else
    digitalWrite(ledPinBuiltin, HIGH);

  int ldrReading = 0;
  int luminosity = 0;
  int celsiusTemp = 0;
  int fahrenheitTemp = 0;

//  logReadingTime();

  temperatureReading(celsiusTemp, fahrenheitTemp);
//  temperatureLogging(celsiusTemp, fahrenheitTemp);

  luminosityReading(ldrReading, luminosity);
  luminosityLogging(ldrReading, luminosity);

  
  JsonObject& json = buildJson(celsiusTemp, ldrReading);

  json.prettyPrintTo(Serial);
  Serial.println();
  json.printTo(Serial);
  Serial.println();

  delay(calculateDelayTime());
}

/*
 * Para desenvolvimento/controle.
 */
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

/*
 * Leitura do sensor de temperatura LM35z.
 * 
 * Faz 8 medidas para melhor precisão e calcula a média (ao longo de 800ms).
 * Converte para Farenheit.
 */
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

/*
 * Para desenvolvimento/controle.
 */
void temperatureLogging(int& celsius, int& farenheit) {
  Serial.print("Temperatura: ");
  Serial.print(celsius, DEC);
  Serial.print(" \u00B0C, ");
  Serial.print(farenheit, DEC);
  Serial.println(" \u00B0F.");
  Serial.println();
}

/*
 * Leitura do sensor de luminosidade. (0 ~ 1023)
 * Baseando leitura na resistência encontrada.
 * 
 * Para definir uma escala em LUX, seria necessário um sensor de LUX para estabelecer uma 
 * correlação entre as tensões e valores de LUX (que cresce em escala logaritmica).
 */
void luminosityReading(int& state, int& luminosity) {
  state = analogRead(ldrPin);
  luminosity = map(state, 0, 1023, 0, 255);
}

void luminosityLogging(int& state, int& luminosity) {
//  Serial.print("Valor lido do LDR: ");
//  Serial.println(state);
//
//  Serial.print("Luminosidade: ");
//  Serial.println(luminosity);
//  Serial.println();

  if (state > 800)
    analogWrite(ledPin, 1023);
  else if (state < 100)
    analogWrite(ledPin, 0);
  else
    analogWrite(ledPin, luminosity);
}

JsonObject& buildJson(int& celsius, int& luminosity) {
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["temperature"] = celsius;
  root["luminosity"] = luminosity;
  
  return root;
}

long calculateDelayTime() {
  return (long) (timeInterval * millisToMinute) - millis() % (long)(timeInterval * millisToMinute);
}

