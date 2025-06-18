#include <stdio.h>

// ARDUINO TO PROCESSING
// This sketch reads analog inputs from one joystick (X, Y) and a push button (Clear).
// It sends Serial data to Processing whenever the cursor moves.
//
// Data format:
//    - On movement: "x_cur,y_cur\n"
//    - On clear button press: "clear\n"
//
// Baud Rate: 9600 (match in Processing)

// PIN CONFIGURATION
const int x_pin = A0; // JOYSTICK VRX
const int y_pin = A1; // JOYSTICK VRY
const int clear_button = 3;

// POSITION TRACKING
int x_cur = 0;
int y_cur = 0;
int x_prev = 0;
int y_prev = 0;

// STATE FLAG
bool clear_requested = false;

void setup() {
    // Initialize pins; Put analog pins in input mode
    pinMode(x_pin, INPUT);
    pinMode(y_pin, INPUT);

    // Put digital pin in input mode (default: high)
    pinMode(clear_button, INPUT_PULLUP);

    // Start Serial communication
    Serial.begin(9600);
}

// Send current position or clear command over Serial
/*
  In Processing:

  String data = myPort.readStringUntil('\n');
  // Parse 'data' for "clear" or x,y coordinates
*/
void print_pos() {
    if (clear_requested) {
        Serial.println("clear");  // Send clear command
        delay(200);
        clear_requested == false;
    } 
    if (x_cur != 0 || y_cur != 0) {
        Serial.print(x_cur);
        Serial.print(",");
        Serial.println(y_cur);   // Send position as x,y
    }
}

void loop() {
    // Placeholders
    x_prev = x_cur;
    y_prev = y_cur;

    // Read from potentiometers connected to analog pins
    int x_read = analogRead(x_pin);
    int y_read = analogRead(y_pin);
   
    const int centre = 512;
    const int th = 20;

    // Update current position based on new joystick position
    if (x_read < centre - th){
       x_cur -= 10;
    } else if (x_read > centre + th){
       x_cur += 10;
    }
    if (y_read < centre - th){
       y_cur -= 10;
    } else if(y_read > centre + th){
       y_cur += 10;
    }

    // Keep cur within bounds  
    x_cur = constrain(x_cur,0,350);
    y_cur = constrain(y_cur,0,350);

    // Invert value of button (FALSE = pushed, TRUE = not pushed)
    if(digitalRead(clear_button) == HIGH && clear_requested == false){
        clear_requested = true;
        x_cur = 0;
        y_cur = 0;
    }
    else {
        clear_requested = false;
        Serial.println("no clear");
    }

    // Print new position to serial monitor if position values have changed
    if ((x_prev != x_cur || y_prev != y_cur) || clear_requested) {
        print_pos();
    }

    // Wait for short time before looping
    delay(100);
}
