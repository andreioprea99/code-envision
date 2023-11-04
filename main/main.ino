#include "DHT.h"
#include <MQUnifiedsensor.h>

// Temperature definitions
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

// MQ 135
#define MQPIN 13
#define BOARD "ESP-32"
#define VOLTAGE_RESOLUTION 5
#define SENSOR_TYPE "MQ-135"
#define ADC_Bit_Resolution 12
#define RatioMQ135CleanAir 3.6  

MQUnifiedsensor MQ135(BOARD, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println(F("DHTxx test!"));

  dht.begin();
  setupMQ();
}

void setupMQ() {
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135.setA(110.47); 
  MQ135.setB(-2.862);

  MQ135.init();    
  Serial.print("Calibrating please wait.");   
  float calcR0 = 0;   
  for(int i = 1; i<=10; i ++)   {     
      MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin     
      calcR0 += MQ135.calibrate(RatioMQ135CleanAir);    
      Serial.print(".");   
  }   
  MQ135.setR0(calcR0/10);   
  Serial.println("  done!.");      
  if(isinf(calcR0)) { Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}   
  if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
}

double readCO2() {
  MQ135.setA(110.47); 
  MQ135.setB(-2.862);
  MQ135.update();
  
  return MQ135.readSensor();
}

double readCO() {
  MQ135.setA(605.18); 
  MQ135.setB(-3.937);
  MQ135.update();
  
  return MQ135.readSensor();
}

double readAlcohool() {
  MQ135.setA(77.255); 
  MQ135.setB(-3.18);
  MQ135.update();
  
  return MQ135.readSensor();
}

double readToluen() {
  MQ135.setA(44.947); 
  MQ135.setB(-3.445);
  MQ135.update();
  
  return MQ135.readSensor();
}

double readNH4() {
  MQ135.setA(102.2); 
  MQ135.setB(-2.473);
  MQ135.update();
  
  return MQ135.readSensor();
}

double readAceton() {
  MQ135.setA(34.668); 
  MQ135.setB(-3.369);
  MQ135.update();
  
  return MQ135.readSensor();
}

void loop(){
  delay(500);

  float h = readHumidity();
  float t = readTemperature();
  float hic = heatIndex(t, h);

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    //return;
  }

  Serial.print(F("CO2: "))
  Serial.println(readCO2)

  Serial.print(F("CO: "))
  Serial.println(readCO)
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