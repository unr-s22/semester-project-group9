#include <LiquidCrystal.h>

volatile unsigned char *myADMUX = (unsigned char *)0x7C;
volatile unsigned char *myADCSRA = (unsigned char *)0x7A;
volatile unsigned char *myADCH = (unsigned char *) 0x79;
volatile unsigned char *myADCL = (unsigned char *)0x78;

LiquidCrystal lcd(1, 2, 4, 5, 6, 7);

void setup() {
  
  lcd.begin(16, 2);
  lcd.print("WATER LEVEL: ");
}

void loop()
{

  *myADMUX |= B00000101; // Read A5
  *myADMUX |= B11000000; //REFS1 and REFS0 to 1 to read from internal volt reference
  *myADCSRA |= B11000000; //ADEN and ADSC set to 1 to begin conversion

  while (bit_is_set(*myADCSRA, *myADCSRA >> 6));

  int value = *myADCL | (*myADCH << 8);
  lcd.setCursor(0, 1);
  if (value <= 400)
  {
    lcd.print("Empty           ");
  }
  else
  {
    lcd.print("In Water            ");
  }
  

  delay(1000);

}
