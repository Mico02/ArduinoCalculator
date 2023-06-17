#include <LiquidCrystal.h>
#include <IRremote.hpp>
#include "Parser.h"
//LCD Arduino Pin Numbers
#define RS 2 
#define EN 3
#define D0 4 
#define D1 5
#define D2 6
#define D3 7
#define D4 8
#define D5 9
#define D6 10
#define D7 11

//IR Receiver input pin
#define IR_INPUT_PIN 12 

//IR hexcode for certain buttons on IR Remote
#define ZERO 0x16
#define ONE 0xC
#define TWO 0x18
#define THREE 0x5E
#define FOUR 0x8
#define FIVE 0x1C
#define SIX 0x5A
#define SEVEN 0x42
#define EIGHT 0x52
#define NINE 0x4A
#define POWER 0x45
#define PLUSIR 0x46
#define MINUSIR 0x15
#define PLAYPAUSEIR 0x40 


//Creating LCD object
LiquidCrystal lcd(RS, EN, D0, D1, D2, D3, D4, D5, D6, D7); 


//Initilzaing value that represents where the next free space in LCD is
uint8_t lcdPos = 0;

/**
 * Intializing a 16 column, 2 row lcd display, clears any previous data on it, 
 * and sets the cursor to the origin.
 */
void initializeLCD(){
    lcd.begin(16,2);
    lcd.clear();
    lcd.setCursor(0,0);
}

void displayChar(){
  lcd.setCursor(lcdPos%16,lcdPos/16);           //Set the LCD cursor based of what lcdPos is.
  switch(IrReceiver.decodedIRData.command){
      case ZERO:
         lcd.print("0");
         lcdPos++;
         break;
      case ONE:
         lcd.print("1");
         lcdPos++;
         break;
      case TWO:
         lcd.print("2");
         lcdPos++; 
         break;
      case THREE:
         lcd.print("3");
         lcdPos++;
         break;
      case FOUR:
         lcd.print("4");
         lcdPos++;
         break;
      case FIVE:
         lcd.print("5");
         lcdPos++;
         break;
      case SIX:
         lcd.print("6");
         lcdPos++;
         break;
      case SEVEN:
         lcd.print("7");
         lcdPos++;
         break;
      case EIGHT:
         lcd.print("8");
         lcdPos++;
         break;
      case NINE:
         lcd.print("9");
         lcdPos++;
         break;
      case PLUSIR: 
         lcd.print("+");
         lcdPos++;
         break;
      case MINUSIR: 
         lcd.print("-");
         lcdPos++;
         break;
      case PLAYPAUSEIR: 
         lcd.print("=");
         lcdPos++;
         break;
       default:
         lcd.clear();
         lcdPos = 0;
         break;
    }
}
void setup(){
    initializeLCD(); //Initialize LCD
    IrReceiver.begin(IR_INPUT_PIN); //Begin reading IR Data 
}
void loop(){
   lcdPos= (lcdPos >= 16) ? 0 : lcdPos;              //If the cursor position is out of bounds, reset back to first position.   
   if(IrReceiver.decode()){
      displayChar();
      IrReceiver.resume();
   }
    
}