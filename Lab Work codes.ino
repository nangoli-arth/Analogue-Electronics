#include <Servo.h>

Servo servo1;  // create servo object to control a servo
Servo servo2;

const int buttonPin = 6; //used to be 2
int buttonState = 0;
int lastButtonState = 0; // for detecting button state changes
bool doorOpen = false;   // state to keep track of door position


// Define limit switches on pins 7 and 6
const int limitSwitchOpenPin = 8; 
const int limitSwitchClosePin = 7;

// Track the current position of the servos
int currentPosition = 0;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);

  // Set limit switch pins as input with internal pull-ups
  pinMode(limitSwitchOpenPin, INPUT_PULLUP);
  pinMode(limitSwitchClosePin, INPUT_PULLUP);

  servo1.attach(9);  
  servo2.attach(10); 

  // Initialize servos to closed position
  servo1.write(currentPosition);  
  servo2.write(currentPosition);

  Serial.println("Setup complete. Monitoring limit switches...");
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Read limit switch states
  int limitSwitchOpenState = digitalRead(limitSwitchOpenPin);
  int limitSwitchCloseState = digitalRead(limitSwitchClosePin);

  // Print limit switch states to the Serial Monitor
  Serial.print("Limit Switch Open State: ");
  Serial.println(limitSwitchOpenState == LOW ? "PRESSED" : "RELEASED");
  
  Serial.print("Limit Switch Close State: ");
  Serial.println(limitSwitchCloseState == LOW ? "PRESSED" : "RELEASED");

  // Check if the button is pressed (and handle toggle)
  if (buttonState != lastButtonState) { // if button state has changed
    if (buttonState == HIGH) {          // if button is now pressed
      doorOpen = !doorOpen;             // toggle door state
      
      if (doorOpen) {
        Serial.println("Button Pressed - Door opening...");
        openDoors();
      } else {
        Serial.println("Button Pressed - Door closing...");
        closeDoors();
      }
    }
    delay(50);  // simple debounce
  }

  lastButtonState = buttonState;
  delay(500); // Slow down loop for readability in Serial Monitor
}

void openDoors() {


  while (currentPosition < 180) {
    // Stop if open limit switch is triggered
    if (digitalRead(limitSwitchOpenPin) == LOW) { 
      Serial.println("Open Limit Switch Triggered - Stopping");
      break;
    }
    servo1.write(currentPosition);
    servo2.write(currentPosition);
    currentPosition++;  // Move to the next angle
    delay(15); 
  }
}

void closeDoors() {

  while (currentPosition > 0) {
    // Stop if close limit switch is triggered
    if (digitalRead(limitSwitchClosePin) == LOW) { 
      Serial.println("Close Limit Switch Triggered - Stopping");
      break;
    }
    servo1.write(currentPosition);
    servo2.write(currentPosition);
    currentPosition--;  // Move to the previous angle
    delay(15); 
  }
}
