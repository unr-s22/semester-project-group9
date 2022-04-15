//Ryan Purciel, for the CPE 301 Semester Project
//Header file to drive the LCD and make it print messages
//4-14-2022
#include <LiquidCrystal.h>
//#include "MasterPinList.h"

/* 
*	Pins are labelled the same that they are on the LCD header. Connect the defined pin to that pin on the LCD.
*/
#define RS_PIN_LCD 12
#define E_PIN_LCD 11
#define D4_PIN_LCD 5
#define D5_PIN_LCD 4
#define D6_PIN_LCD 3
#define D7_PIN_LCD 2

LiquidCrystal lcd(RS_PIN_LCD, E_PIN_LCD, D4_PIN_LCD, D5_PIN_LCD, D6_PIN_LCD, D7_PIN_LCD);

void LCDMessage(String line1, String line2){

  lcd.begin(16, 2);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);

}
