// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <dht11.h>
//#include "MasterPinList.h"

#define SENSOR_PIN_DHT11 52

dht11 DHT;

float readTemperature(){
  int pin = DHT.read(SENSOR_PIN_DHT11);
  
  return DHT.temperature;
}

float readHumidity(){
  int pin = DHT.read(SENSOR_PIN_DHT11);
  
  return DHT.humidity;
}
