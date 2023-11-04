#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "POCO F3";
const char* password = "52b18ccf75fb";

#include "DHT.h"

#define SLEEP_TIME 30000 
// Temperature definitions
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

float h, t, hic;

// MQ 135
#define MQPIN 34

int ppm;

// DSM
#define PM1PIN 12
#define PM25PIN 14

unsigned long durationPM1;
unsigned long durationPM25;
unsigned long elapsedtime;
unsigned long endtime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancyPM1 = 0;
unsigned long lowpulseoccupancyPM25 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  dht.begin();
  setupDSM();
  setupwifi();
}

void setupDSM() {
  pinMode(PM1PIN,INPUT);
  pinMode(PM25PIN,INPUT);
}

void loop(){
  
  h = readHumidity();
  t = readTemperature();
  hic = heatIndex(t, h);

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }

  ppm = analogRead(MQPIN);
  Serial.println(F("PPM: "));
  Serial.println(ppm);

  readDSM();
}

float calculateConcentration(long lowpulseInMicroSeconds, long durationinSeconds){
  
  float ratio = (lowpulseInMicroSeconds/1000000.0)/30.0*100.0; //Calculate the ratio
  float concentration = 0.001915 * pow(ratio,2) + 0.09522 * ratio - 0.04884;//Calculate the mg/m3
  Serial.print("lowpulseoccupancy:");
  Serial.print(lowpulseInMicroSeconds);
  Serial.print("    ratio:");
  Serial.print(ratio);
  Serial.print("    Concentration:");
  Serial.println(concentration);
  return concentration;
}

void readDSM(){
  elapsedtime = millis();
  endtime = elapsedtime + sampletime_ms;
  
  lowpulseoccupancyPM1 = 0;
  lowpulseoccupancyPM25 = 0;

  if (endtime < elapsedtime) {
    Serial.println("missing");
    return;
  }
  
  while (elapsedtime < endtime) //Only after 30s has passed we calcualte the ratio
  {
    durationPM1 = pulseIn(PM1PIN, LOW);
    durationPM25 = pulseIn(PM25PIN, LOW);

    lowpulseoccupancyPM1 += durationPM1;
    lowpulseoccupancyPM25 += durationPM25;

    elapsedtime = millis();    
  }
      /*
    ratio1 = (lowpulseoccupancy/1000000.0)/30.0*100.0; //Calculate the ratio
    Serial.print("ratio1: ");
    Serial.println(ratio1);
    
    concentration = 0.001915 * pow(ratio1,2) + 0.09522 * ratio1 - 0.04884;//Calculate the mg/m3
    */
    float conPM1 = calculateConcentration(lowpulseoccupancyPM1,30);
    float conPM25 = calculateConcentration(lowpulseoccupancyPM25,30);
    Serial.print("PM1 ");
    Serial.print(conPM1);
    Serial.print("  PM25 ");
    Serial.println(conPM25);
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

void setupwifi(){
  
  Serial.begin(9600); 
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
