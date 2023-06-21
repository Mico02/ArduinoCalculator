#include <LiquidCrystal.h>
#include <IRremote.hpp>
#include "Evaluator.h"
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
#define VOLPLUS 0x46        //'+'
#define VOLMINUS 0x15       //'-'
#define PLAYPAUSE 0x40      //'='
#define REWIND 0x44         //'*'
#define FORWARD 0x43        //'/'
#define EQ 0x19             //'(' 
#define ST 0xD              //'}'
uint16_t numbers[10] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

//Creating LCD object
LiquidCrystal lcd(RS, EN, D0, D1, D2, D3, D4, D5, D6, D7); 


//Initilzaing value that represents where the next free space in LCD is
uint8_t lcdPos = 0;

//Initializing array that will store the arithmatic expression
int16_t expression[16];

//Flag that represents whether or not operation is complete
bool operationComplete = false;
/**
 * Intializing a 16 column, 2 row lcd display, clears any previous data on it, 
 * and sets the cursor to the origin.
 */
void initializeLCD(){
    lcd.begin(16,2);
    lcd.clear();
    lcd.setCursor(0,0);
}

char displayChar(){
  lcd.setCursor(lcdPos%16,lcdPos/16);           //Set the LCD cursor based of what lcdPos is.
  switch(IrReceiver.decodedIRData.command){
      case ZERO:
         lcd.print("0");
         lcdPos++;
         return '0';
      case ONE:
         lcd.print("1");
         lcdPos++;
         return '1';
      case TWO:
         lcd.print("2");
         lcdPos++; 
         return '2';
      case THREE:
         lcd.print("3");
         lcdPos++;
         return '3';
      case FOUR:
         lcd.print("4");
         lcdPos++;
         return '4';
      case FIVE:
         lcd.print("5");
         lcdPos++;
         return '5';
      case SIX:
         lcd.print("6");
         lcdPos++;
         return '6';
      case SEVEN:
         lcd.print("7");
         lcdPos++;
         return '7';
      case EIGHT:
         lcd.print("8");
         lcdPos++;
         return '8';
      case NINE:
         lcd.print("9");
         lcdPos++;
         return '9';
      case VOLPLUS: 
         lcd.print("+");
         lcdPos++;
         return '+';
      case VOLMINUS: 
         lcd.print("-");
         lcdPos++;
         return '-';
      case REWIND:
         lcd.print("*");
         lcdPos++;
         return '*';
      case FORWARD:
         lcd.print("/");
         lcdPos++;
         return '/';
      case PLAYPAUSE: 
         lcd.print("=");
         lcd.setCursor(0,1);
         return '=';
      case EQ:
         lcd.print("(");
         lcdPos++;
         return '(';
      case ST:
         lcd.print(")");
         lcdPos++;
         return ')';
       default:
         lcd.clear();
         lcdPos = 0;
         return '\n';
    }
}
/**
 * Checks if the given IR command corressponds with a digit.
*/
bool isDigit(uint16_t command){
 for(int i = 0; i<10; i++){
   if(numbers[i] == command){
     return true;
   }
  }
  return false;
}


/**
 * Checks if the given IR command corressponds with arthimatic operation.
*/
bool isOperation(uint16_t command){
   return command == VOLPLUS || command == VOLMINUS || command == REWIND || command == FORWARD
          || command == EQ || command == ST;
}

int getOperationCode(uint16_t command){
   switch (command)
   {
   case VOLPLUS:
      return PLUS;
   case VOLMINUS:
      return MINUS;
   case REWIND:
      return TIMES;
   case FORWARD:
      return DIVIDE;
   default:
      return 0; //Returns Zero if invalid operation
   }
}


/**
 * Converts the command received from the IR remote into its digit
 * as an int.
*/
int getDigit(uint16_t command){
   for(int i = 0; i<10; i++){
      if(numbers[i] == command){
         return i;
      }
  }
  return -1;
}

void printArray(int16_t a[], uint8_t length){
   for(int i = 0; i < length;i++){
      Serial.print(a[i]);
      Serial.print(" ");
   }
   Serial.println();
}

void truncateArray(int16_t array[], uint8_t n){
   int16_t newArr[n];
   for(int i = 0; i < n; i++){
      newArr[i] = array[i];
   } 
   printArray(newArr,3);
   array = newArr;
}

int16_t getNumber(){
   uint8_t i = 1;
   int16_t number = 0;
   while(isDigit(IrReceiver.decodedIRData.command)){
      number += getDigit(IrReceiver.decodedIRData.command)*i;
      i *= 10;
      IrReceiver.resume();
   }
   
   return number;
}


void setup(){
    initializeLCD(); //Initialize LCD
    IrReceiver.begin(IR_INPUT_PIN); //Begin reading IR Data 
    Serial.begin(115200);
    int16_t n[] = {12, MINUS, 3}; 
    Serial.println(valueOfExpr(n));
}
void loop(){
  int value = 0;
  lcdPos = (lcdPos >= 16) ? 0 : lcdPos;              //If the cursor position is out of bounds, reset back to first position.  


  
    
  if(IrReceiver.decode() && !operationComplete){
   uint16_t command = IrReceiver.decodedIRData.command;
   if(isDigit(command)){
      expression[lcdPos] = getNumber();
   } else if(isOperation(command)){
      expression[lcdPos] = getOperationCode(command);
   } else if(command == EQ || command == ST){
      expression[lcdPos] = (command == EQ) ? LEFTPARENTHESE : RIGHTPARENTHESE;
   } else if(command == PLAYPAUSE){
      truncateArray(expression, lcdPos);
      value = valueOfExpr(expression);
      operationComplete = true;
   } else { //IF any other button is pressed, clear the calculator.
      value = 0;
   }
   displayChar();
   IrReceiver.resume();
  }

 if(operationComplete){
    printArray(expression,16);
    Serial.print("***LCDPOS ");
    Serial.print(lcdPos);
    lcd.print(value);
    for(;;);
 }   
  
  delay(300);
   
   
    
}