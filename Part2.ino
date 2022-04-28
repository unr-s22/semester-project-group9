#include <LiquidCrystal.h>
#include <Stepper.h>
#include <DFRobot_DHT11.h>
#include <RTClib.h>

#define STEPS 500
#define DHT11_PIN 52
#define DHTTYPE DHT11 

class WaterLevelSensor;
class FanMotor;
class SwampCooler;

//pins
const int ButtonPin = 8;

const int LCD_RS = 2;
const int LCD_E = 3;
const int LCD_D4 = 4;
const int LCD_D5 = 5;
const int LCD_D6 = 6;
const int LCD_D7 = 7;

const int STEPPER_N1 = 10;
const int STEPPER_N2 = 11;
const int STEPPER_N3 = 12;
const int STEPPER_N4 = 13;

const int FAN_ENABLE = 46;
const int FAN_DIRA = 48;
const int FAN_DIRB = 50;

const int SENSOR_PIN = B00000100;

const int BUTTON_DISABLE_PIN = 30;
const int BUTTON_ENABLE_PIN = 19;

const int GREEN_LED = 34;
const int RED_LED = 38;
const int YELLOW_LED = A6;
const int BLUE_LED = A7;

//state
int buttonState = 0;

//Modules
//LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
//Stepper stepper(STEPS, STEPPER_N1, STEPPER_N2, STEPPER_N3, STEPPER_N4);

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

class FanMotor {

    public:
      FanMotor(){  
        DDRL |= B00001010;
        DDRB |= B00001000;}
      void turnOn()
      {
          PORTL |= B00001010;
          PORTB &= !B00000000;
      }
      void turnOff()
      {
        PORTL &= !B00000010;
      }
  
  
  };

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Stepper stepper(STEPS, STEPPER_N1, STEPPER_N2, STEPPER_N3, STEPPER_N4);
DFRobot_DHT11 DHT;
FanMotor fan;
WaterLevelSensor sensor(SENSOR_PIN);
RTC_DS1307 rtc;

char currState = 'I';
unsigned long timeElapsed = millis();
bool isStarting = true;
volatile int buttonFlag;

 void ISR_Enable()
{
  buttonFlag = 1;
}

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {

  pinMode(BUTTON_DISABLE_PIN, INPUT);
  pinMode(BUTTON_ENABLE_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_ENABLE_PIN), ISR_Enable, HIGH);



  
  DDRH |= B00100000;

  lcd.begin(16, 2);
  stepper.setSpeed(30);

  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
    }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void loop()
{
    DateTime now = rtc.now();
    int value = sensor.getReading();

    if (digitalRead(BUTTON_DISABLE_PIN) == HIGH)
    {
      Serial.print("Disabled state entered at: ");
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(' ');
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.println();

      currState = 'D';
    }

    
    if (currState != 'D')
    {
      digitalWrite(YELLOW_LED, LOW);

      if (millis() - timeElapsed >= 60000 || isStarting == true)
      {

        if (isStarting == true) {isStarting = false;}
        DHT.read(DHT11_PIN);

        lcd.setCursor(0,0);
        lcd.print("Temperature: ");
        lcd.print(DHT.temperature);
        lcd.setCursor(0,1);
        lcd.print("Humidity: ");
        lcd.print(DHT.humidity);
        timeElapsed = millis();
      }

//        DHT.read(DHT11_PIN);
//
//        lcd.setCursor(0,0);
//        lcd.print("Temperature: ");
//        lcd.print(DHT.temperature);
//        lcd.setCursor(0,1);
//        lcd.print("Humidity: ");
//        lcd.print(DHT.humidity);

        
      if (currState == 'I')
      {

        fan.turnOff();
        
        digitalWrite(GREEN_LED, HIGH);

        if (value <= 700)
        {
         
          Serial.print("Error state entered at: ");
          Serial.print(now.year(), DEC);
          Serial.print('/');
          Serial.print(now.month(), DEC);
          Serial.print('/');
          Serial.print(now.day(), DEC);
          Serial.print(' ');
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
          Serial.println();
          digitalWrite(GREEN_LED, LOW);
          currState = 'E';
          
        }
       
        if (currState == 'I' && DHT.temperature >= 28)
        {
          Serial.print("Running state entered at: ");
          Serial.print(now.year(), DEC);
          Serial.print('/');
          Serial.print(now.month(), DEC);
          Serial.print('/');
          Serial.print(now.day(), DEC);
          Serial.print(' ');
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
          Serial.println();
          digitalWrite(GREEN_LED, LOW);
          currState = 'R';
        }
      }

      if (currState == 'E')
      {

        lcd.setCursor(0,0);
        lcd.print("Error. Water low");
        lcd.setCursor(0,1);
        lcd.print("                ");
        
        digitalWrite(RED_LED, HIGH);
        fan.turnOff();

        if (value > 700)
        {
          currState = 'I';

          Serial.print("Idle state entered at: ");
          Serial.print(now.year(), DEC);
          Serial.print('/');
          Serial.print(now.month(), DEC);
          Serial.print('/');
          Serial.print(now.day(), DEC);
          Serial.print(' ');
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
          Serial.println();
          digitalWrite(RED_LED, LOW);
          isStarting = true;

          lcd.clear();
        }
      }

      if (currState == 'R')
      {
        fan.turnOn();

        digitalWrite(BLUE_LED, HIGH);


        if (value <= 700)
        {
         
          Serial.print("Error state entered at: ");
          Serial.print(now.year(), DEC);
          Serial.print('/');
          Serial.print(now.month(), DEC);
          Serial.print('/');
          Serial.print(now.day(), DEC);
          Serial.print(' ');
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
          Serial.println();
          digitalWrite(GREEN_LED, LOW);
          currState = 'E';

          digitalWrite(BLUE_LED, LOW);
          
        }

        if (currState == 'R' && DHT.temperature < 28)
        {
          currState = 'I';

          Serial.print("Idle state entered at: ");
          Serial.print(now.year(), DEC);
          Serial.print('/');
          Serial.print(now.month(), DEC);
          Serial.print('/');
          Serial.print(now.day(), DEC);
          Serial.print(' ');
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
          Serial.println();
          digitalWrite(BLUE_LED, LOW);
        }
      }
    }
    else
    {
      digitalWrite(YELLOW_LED, HIGH);

      if (buttonFlag)
      {
        buttonFlag = 0;
        digitalWrite(YELLOW_LED, LOW);
        currState = 'I';
        isStarting = true;
      }
    }

    buttonState = (PINH & B00100000) >> 5;

    if (buttonState == HIGH)
    {
      stepper.step(100);
      Serial.print("Stepper Motor changed on : " );
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(' ');
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.println();
    }
        

  
  
  delay(300);

  }
