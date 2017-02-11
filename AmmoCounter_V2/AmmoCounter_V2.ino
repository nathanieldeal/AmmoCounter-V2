// AmmoCounter V2 - www.ammocounter.com   
// Updated 1/21/2017
// Created by: Nathaniel Deal

// Include Libraries
#include <Button.h>

//  2 Digit 7 Segment Common Anode 10 Pin Out

    /*        A  B Ca1 Ca2 F
              |  |  |   |  |
         ---------    ---------
         |   A   |    |   A   |
        F|       |B  F|       |B
         |---G---|    |---G---|
        E|       |C  E|       |C
         |   D   |    |   D   |
         ---------    ---------
              |  |  |  |  |
              C  Dp E  D  G  
    */

// 7 Segment Pin Connections
byte A = 12;         // Set A to Pin 12
byte B = 11;         // Set B to Pin 11
byte C = 10;         // Set C to Pin 10
byte D = 3;          // Set D to Pin 3
byte E = 4;          // Set E to Pin 4
byte F = 5;          // Set F to Pin 5
byte G = 6;          // Set G to Pin 6

int pinArray[10]={A,B,C,D,E,F,G}; // Setup array of pins

byte pnp1 = 13;  // PNP Transisitor Base Pin 13
byte pnp2 = 2;   // PNP Transisitor Base Pin 12

// Setup array of digits
byte segmentDigits[11][7] = { 
  { 0,0,0,0,0,0,1 },  // 0
  { 1,0,0,1,1,1,1 },  // 1
  { 0,0,1,0,0,1,0 },  // 2
  { 0,0,0,0,1,1,0 },  // 3
  { 1,0,0,1,1,0,0 },  // 4
  { 0,1,0,0,1,0,0 },  // 5
  { 0,1,0,0,0,0,0 },  // 6
  { 0,0,0,1,1,1,1 },  // 7
  { 0,0,0,0,0,0,0 },  // 8
  { 0,0,0,1,1,0,0 }   // 9
};

// Setup Toggle/Counter Variables
int toggleArray[] = {0,35,25,22,18,15,12,10,6}; // Dart clip sizes (Use {0,99,45,40} For Khaos)
int toggleCount = (sizeof(toggleArray)/sizeof(int))-1; // Find size of array
int togglePosition = toggleCount; // Start at max capacity
int count = toggleArray[toggleCount];  // Set intial count to lowest capacity
int firstDigit, secondDigit;

// IR Beam Setup
int irSensorPin = A2;
int idleValue = 1;
int fireValue = 3;
boolean hasCleared = false;  // Check for cleared dart

// Clip/Toggle/Reset Setup
Button toggleBtn = Button (8, PULLDOWN);   // Use digital pin 8 for the toggle pin
Button resetBtn = Button (9, PULLDOWN);    // Use digital pin 9 for the reset pin

void setup() {

  // Setup pnp transitor pins
  digitalWrite(pnp1, HIGH); // Pull Up
  digitalWrite(pnp2, HIGH); // Pull Up
  pinMode(pnp1, OUTPUT);
  pinMode(pnp2, OUTPUT);

  // Setup LED pins
  pinMode(A, OUTPUT);   
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  
  // Set Initial Count
  changeNumber(count);

  // Uncomment for testing
  // Serial.begin(9600); 
}               

void loop(){
  
  // Display Digits    
    showDigits();

  // Monitor IR Beam
  //----------------------------------------------------//

    // Read the analog in value
    int sensorValue = analogRead(irSensorPin); 

    // Map it to the range of the analog output
    int outputValue = map(sensorValue, 0, 1023, 0, 10);  
    
    // Check to see if dart has fired
    if (outputValue > fireValue)
    {
      if (hasCleared == true) // If barrel is clear and beam is broken then countdown 
      {

        if (toggleArray[togglePosition] == 0) { 

          // If OO has been selected, Count Up
          changeNumber(++count);
          hasCleared = false;

          // Check if count has finished, Auto-Reset
          if (count == 99) {
            _autoReset(); // Reset Count
          }
          
        } else {

          // Count Down
          changeNumber(--count);
          hasCleared = false;
  
          // Check if count has finished, Auto-Reset
          if (count == 0) {
            _autoReset(); // Reset Count
          }
        }

        // Print the results to the serial monitor for testing
        // Serial.print("\t output = ");      
        // Serial.println(outputValue);
      }
    }

    // Check to see if dart has cleared
    if (outputValue <= idleValue)
    {
      hasCleared = true;
    }

    
  // Monitor Toggle Button
  //----------------------------------------------------//
  
    // Check if the togglebutton is pressed.
    if (toggleBtn.uniquePress()) {       
        
      // Toggle the display
      if (togglePosition == 0) {
        togglePosition = toggleCount; //Reset to max.
      } else {
        togglePosition--; //Deincrement capacity one step 
      } 
      
      count = toggleArray[togglePosition];
      changeNumber(count); //Send to display      
    }
  
  // Monitor Reset Button
  //----------------------------------------------------//
  
    // Check if resetbutton is pressed.
    if (resetBtn.uniquePress()) {      
      count = toggleArray[togglePosition]; // Reset count
      changeNumber(count); //Send to display  
    }
  
}

// Update display count
//----------------------------------------------------//  
void changeNumber(int displayCount) {

  if ( (displayCount < 100) && (displayCount > 9) ) {
    firstDigit = displayCount / 10; // Find the first digit
    secondDigit = displayCount % 10; 
  }

  else if ( (displayCount < 10) && (displayCount > 0) ) {
    firstDigit = 0; // Set the first digit to 0
    secondDigit = displayCount;
  }
  
  else if ( displayCount == 0) {
    firstDigit = 0; // Set the first digit to 0
    secondDigit = 0; // Set the second digit to 0
  }

}

// 7 Segment LED display toggle
//----------------------------------------------------//  
void showDigits() {
  _clearDisplay();
  digitalWrite(pnp2, HIGH);
  digitalWrite(pnp1, LOW);
  _displayNumber(firstDigit);
  delay(5);
  
  _clearDisplay();
  digitalWrite(pnp1, HIGH);
  digitalWrite(pnp2, LOW);
  _displayNumber(secondDigit);
  delay(5);
}

// Given a number, turns on those segments
//----------------------------------------------------//  
void _displayNumber(byte digit) {
  for (byte i = 0; i < 7; ++i) {
    digitalWrite(pinArray[i], segmentDigits[digit][i]);
  }
}

// Clear all all register pins
//----------------------------------------------------//  
void _clearDisplay() {
  for (byte i = 0; i < 7; ++i) {
    digitalWrite(pinArray[i], 1);
  }
} 

// Blink display and then auto-reset     
//----------------------------------------------------//  
void _autoReset() {
  
  // Blink display
  _clearDisplay();
  delay(500);
  
  count = toggleArray[togglePosition]; // Reset count  
  changeNumber(count); //Send to display
}
