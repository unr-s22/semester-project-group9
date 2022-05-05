
class WaterLevelSensor;
class SwampCooler;

class WaterLevelSensor {

  private:
    int myPin;
  
  public:
    WaterLevelSensor(int pin) : myPin(pin) {}
    int getReading() {
      int ans = 0;
      
      ADMUX |= myPin; // Read pin
      ADMUX |= B11000000; //REFS1 and REFS0 to 1 to read from internal volt reference
      ADCSRA |= B11000000; //ADEN and ADSC set to 1 to begin conversion

      while (bit_is_set(ADCSRA, ADCSRA >> 6)); // wait for conversion to finish

      ans = ADCL | (ADCH << 8);

      return ans;
    }
};

class SwampCooler{
  private:
    WaterLevelSensor sens;

  public:
    SwampCooler(int waterSensorPin) : sens(waterSensorPin) {}
    void runCooler(){
        Serial.print(sens.getReading());
      }
};

SwampCooler cooler(B00000100);

void setup() {
  Serial.begin(9600);


}

void loop() {
 
 cooler.runCooler();

}
