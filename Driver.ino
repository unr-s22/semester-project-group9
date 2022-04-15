#include "DHT11Driver.h"
#include "LCDDriver.h"

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = readTemperature();
  float humidity = readHumidity();
  int mode = 5;

  LCDMessage("Temp. RH  Status", String(temp, 0) + " F  " + String(humidity, 0) + "% " + getModeString(mode));
  delay(1000);
}

String getModeString(int modeID){
  switch (modeID){
    case 0:
      return "OFF";
    case 1:
      return "ON";
    case 2:
      return "IDLE";
    default:
      return "ERR";
  }
}
