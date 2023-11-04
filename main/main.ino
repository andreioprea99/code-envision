#include "DHT.h"
#include <MQUnifiedsensor.h>

// Temperature definitions
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

float h, t, hic;

// MQ 135
#define MQPIN 13

int ppm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop(){
  delay(500);

  h = readHumidity();
  t = readTemperature();
  hic = heatIndex(t, h);

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }

  ppm = analogRead(MQPIN);
  Serial.Println(F("PPM: "));
  Serial.Println(ppm);
}

float readTemperature() {
  float t = dht.readTemperature();
  
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("C "));

  return t;
}

float readHumidity() {
  float h = dht.readHumidity();

  Serial.print(F("Humidity: "));
  Serial.println(h);

  return h;
}

float heatIndex(float temp, float humidity) {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("F  Heat index: "));
  Serial.println(hic);

  return hic;
}