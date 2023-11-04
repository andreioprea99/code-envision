#include "DHT.h"

// Temperature definitions
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println(F("DHTxx test!"));

  dht.begin();
}

void loop(){
   
  float h = readHumidity();
  float t = readTemperature();
  float hic = heatIndex(t, h);

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    //return;
  }
}

float readTemperature() {
  float t = dht.readTemperature();
  
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));

  return t;
}

float readHumidity() {
  float h = dht.readHumidity();

  Serial.print(F("Humidity: "));
  Serial.print(h);

  return h;
}

float heatIndex(float temp, float humidity) {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);

  return hic;
}