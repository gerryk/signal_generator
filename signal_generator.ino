#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
#include <Encoder.h>
#include "si5351.h"

Si5351 clockgen;

const int BUTTON = 8;
long oldPosition  = -999;
long int step = 1;
long int frequency = 14000000;

Encoder myEnc(5, 6);
LiquidCrystal_I2C lcd(0x27,20,4); //set the LCD address to 0x27 for a 16 chars and 2 line display 

void setup ()  {
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(0, 0); 
  lcd.print("Initialising...");  
  clockgen.init(SI5351_CRYSTAL_LOAD_8PF);
  lcd.setCursor(0, 1); 
  lcd.print("OK!");
  delay (2000);
  pinMode (BUTTON, INPUT);
  digitalWrite (BUTTON, HIGH); // Pull High Restance
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(0, 0); 
  lcd.print("Freq: "); 
  lcd.setCursor(6, 0); 
  lcd.print(frequency); 
  lcd.setCursor(0, 1); 
  lcd.print("Step: ");
  lcd.setCursor(6, 1); 
  lcd.print(step);
  // Set CLK0 to output 14 MHz with a fixed PLL frequency
  clockgen.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  clockgen.set_freq(frequency, SI5351_PLL_FIXED, SI5351_CLK0);
}

void loop () {
  int delta;
  if (!(digitalRead(BUTTON)))  {
    if (step >= 1000000) 
      step = 1;
    else step = step * 10;    
    lcd.setCursor(6, 1); 
    lcd.print(step);
    lcd.setCursor(6+(step/10),1);
    lcd.print("        ");
    delay (200);
  }
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    delta = newPosition > oldPosition? 1 : -1;
    oldPosition = newPosition;
  frequency = frequency + (delta * step);
  lcd.setCursor(6, 0); 
  lcd.print(frequency); 
  clockgen.set_freq(frequency, SI5351_PLL_FIXED, SI5351_CLK0);
  }
}


