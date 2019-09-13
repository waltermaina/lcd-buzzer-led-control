/*
  lcd, buzzer and led control example

 When new serial data arrives, this sketch adds it to a String.
 When a newline is received, the loop checks for the following 
 commands in the string;
       1.0 LED FLASH - Will cause the pink LED to start flashing
       2.0 LED OFF - Will cause the pink LED to go off/stop flashing
       3.0 BUZZER ON - Will cause the buzzer to go on
       4.0 BUZZER OFF - Will cause the buzzer to go off
       5.0 CLEAR LCD - Will cause the LCD text to be removed
       
If the string contents dont match what is in the defined commands 
it prints an unknown command message

This sketch also uses a 16x2 LCD with a Hitachi HD44780 driver to 
display received serial data. The data is of the following format 
when sent from the PC "LCDCRMessage", where;
        1. LCD - is the code to identify messages for display on the LCD
        2. C - is the start column on the LCD
        3. R - is the start row on the LCD
        4. Message - is the actual messaged to be displayed on the LCD

This Sketch is designed to work with the OCTRINSIC development 
board version 2.0 (www.octrinsic.com).

 Created 6 February 2018
 by Walter Maina

 This example code is in the public domain.
 */

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// constants won't change. Used here to set a pin number :
const int ledPin = A2;      // the number of the LED pin
const int buzzerPin = 5;    // the number of the buzzer pin

// Variables will change :
int ledState = LOW;             // ledState used to set the LED
char ledCommand = 0;            // Used to store the LED on or off command from user

int buzzerState = LOW;          // buzzerState used to set the Buzzer
char buzzerCommand = 0;         // Used to store the buzzer on or off command from user

// Generally, you shuould use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 1000;           // interval at which to blink (milliseconds)

// include the LCD library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A1, A0, 8, 9, 10, 11);

// LCD variables
int column;
int row;
String LCDindex = "";
String LCDClearRow = "                "; // Used to clear a single row on the LCD (16 Spaces)

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("hello, Kenya!");
  lcd.setCursor(1, 2);
  lcd.print("LCD ADDITION!");
}

void loop() {
  // parse the string when a newline arrives:
  if (stringComplete) {
  // substring(index) looks for the substring from the index position to the end:
  if (inputString.substring(0,9) == "LED FLASH") {
    Serial.println("The Pink LED Will Now Start Flashing");
    ledCommand = 1; // Time to blink the LED
  }
  else if (inputString.substring(0,7) == "LED OFF") {
    Serial.println("The Pink LED Will Now Stop Flashing");
    ledCommand = 0; // Time to turn the LED off
  }
  else if (inputString.substring(0,9) == "BUZZER ON") {
    Serial.println("The BUZZER Will Go ON Now");
    buzzerCommand = 1; // Time to turn the buzzer ON
  }
  else if (inputString.substring(0,10) == "BUZZER OFF") {
    Serial.println("The BUZZER Will Go OFF Now");
    buzzerCommand = 0; // Time to turn the buzzer OFF
  }
  else if (inputString.substring(0,3) == "LCD") {
    // Get the LCD column and row
    LCDindex += (char)inputString[3];
    column = LCDindex.toInt();
    LCDindex = "";
    LCDindex += (char)inputString[4];
    row = LCDindex.toInt();
    LCDindex = "";
    
    // Print a message to the LCD.;
    // First row message
    if(row == 0){
      lcd.setCursor(column, row);
      lcd.print(LCDClearRow); // Clear the row first
      lcd.setCursor(column, row);
      lcd.print(inputString.substring(5,inputString.length()-1));
      Serial.println("LCD row one message received");
    }
    // Second row message
    else if(row == 1){ 
      lcd.setCursor(column, row);
      lcd.print(LCDClearRow); // Clear the row first
      lcd.setCursor(column, row);
      lcd.print(inputString.substring(5,inputString.length()-1));
      Serial.println("LCD row two message received");
    }
  }
  else if (inputString.substring(0,9) == "CLEAR LCD") {
    lcd.clear();
    Serial.println("The LCD has been cleared");
  }
  else
    Serial.println("UNKNOWN COMMAND !!!!!!!!!!!!!");
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  
  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

// This is the part that handles the LED command from user
  if(ledCommand == 1){
        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW)
          ledState = HIGH;
        else
          ledState = LOW;
    
        // set the LED with the ledState of the variable:
        digitalWrite(ledPin, ledState);
      } 
   else if (ledCommand == 0){  
        // Turn off LED:    
        ledState = LOW;
        digitalWrite(ledPin, ledState);
      }
 
 // This is the part that handles the BUZZER command from user     
  if(buzzerCommand == 1){
        // if the BUZZER is off turn it on and vice-versa:
        if (buzzerState == LOW)
          buzzerState = HIGH;
        else
          buzzerState = LOW;
    
        // set the BUZZER with the buzzerState of the variable:
        digitalWrite(buzzerPin, buzzerState);
      } 
   else if (buzzerCommand == 0){  
        // Turn off BUZZER:    
        buzzerState = LOW;
        digitalWrite(buzzerPin, buzzerState);
      }
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


