#include <LiquidCrystal.h>
#include <Stepper.h>

#define STEPS 500

volatile unsigned char *myADMUX = (unsigned char *)0x7C;
volatile unsigned char *myADCSRA = (unsigned char *)0x7A;
volatile unsigned char *myADCH = (unsigned char *) 0x79;
volatile unsigned char *myADCL = (unsigned char *)0x78;

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

//state
int buttonState = 0;

//Modules
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Stepper stepper(STEPS, STEPPER_N1, STEPPER_N2, STEPPER_N3, STEPPER_N4);


void setup() {
  
  lcd.begin(16, 2);
  lcd.print("WATER LEVEL: ");
  stepper.setSpeed(30);
  DDRH |= B00100000;
  Serial.begin(9600);
}

void loop()
{

  *myADMUX |= B00000100; // Read A4
  *myADMUX |= B11000000; //REFS1 and REFS0 to 1 to read from internal volt reference
  *myADCSRA |= B11000000; //ADEN and ADSC set to 1 to begin conversion

  while (bit_is_set(*myADCSRA, *myADCSRA >> 6));

  int value = ADCL | (ADCH << 8);
  lcd.setCursor(0, 1);
  if (value <= 700)
  {
    lcd.print("Empty           ");
  }
  else
  {
    lcd.print("In Water            ");
  }

  buttonState = (PINH & B00100000) >> 5;

  if (buttonState == HIGH)
  {
    stepper.step(100);
  }
  

  delay(300);

}
