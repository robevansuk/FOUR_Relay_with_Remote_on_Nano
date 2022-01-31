#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <std::map>


// Define sensor pin
const int RECV_PIN             = 4;

// Define LED pin constants
const int redPin               = 5;
const int yellowPin            = 6;
const int bluePin              = 7;
const int greenPin             = 8;
const int pinkPin              = 9;
const int brownPin             = 10;
const int violetPin            = 11;
const int greyPin              = 12;

const int inPin                = 15;        // the number of the input pin
const int outPin               = 16;        // the number of the output pin

// Define IR Receiver and Results Objects
IRrecv irrecv(RECV_PIN);
decode_results results;

// Set the LCD address to 0x27 (not sure what this is) for a 16 char, 4 line display
LiquidCrystal_I2C lcd(0x27, 16, 4);

// the following variables are long's because the time, measured in miliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastUpdateTime   = 0;          // the last time the output pin was toggled
unsigned long debounce         = 200UL;      // the debounce time, increase if the output flickers


//---------------- 
// mutable state
//----------------
int outputState                = HIGH;      // the current state of the output pin
int reading;                                // the current reading from the input pin
int previousReading            = LOW;       // the previous reading from the input pin  

const map<int, bool> relayStatusMap;
relayStatusMap[0] = true;
relayStatusMap[1] = true;
relayStatusMap[2] = true;
relayStatusMap[3] = true;

buttonPins[] = { 5, 6, 7, 8 }; // buttons attach to pins 5, 6, 7 and 8 on the arduino nano diagram corresponding to D2, D3, D4, D5

// I'm not convinced the pin numbers correspond correctly with the diagram.
const map<int, int> pinAddressMap;
pinAddressMap[0xA25D807F] = 0; // redPin;    // relay 1
pinAddressMap[0xA25D40BF] = 1; // yellowPin; // relay 2
pinAddressMap[0xA25DC03F] = 2; // bluePin;   // relay 3
pinAddressMap[0xA25D20DF] = 3; // greenPin;  // relay 4
//pinAddressMap[0xA25DA05F] = pinkPin;   // relay 5
//pinAddressMap[0xA25D609F] = brownPin;  // relay 6
//pinAddressMap[0xA25DE01F] = violetPin; // relay 7
//pinAddressMap[0xA25D10EF] = greyPin;   // relay 8

void setup() {

    {
        pinMode(inPin, INPUT);   // reading / previousReading
        pinMode(outPin, OUTPUT); // state
    }

    Serial.begin(9600);

    // Enable the IR Receiver
    irrecv.enableIRIn();
    
    // Set LED pins as Outputs
    pinMode(redPin, OUTPUT);
    pinMode(yellowPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(pinkPin, OUTPUT);
    pinMode(brownPin, OUTPUT);
    pinMode(violetPin, OUTPUT);
    pinMode(greyPin, OUTPUT);

    // initialize the LCD
    lcd.begin();

    // Turn on the blacklight and print a message.
    lcd.backlight();
}


void loop() {

    checkIRSignals();
    checkButtons();
    updateLcdDisplay();

    // add debounce delay here so we prevent the loop being called again for the next however many milliseconds.
    // delay(150); delayMicroseconds();

}

void checkIRSignals() {
   if (irrecv.decode(&results)) {

        // Serial Monitor @ 9600 baud
        Serial.println("results.value before update : " + results.value, HEX);
        
        int relayId = pinAddressMap[results.value]; // not entirely sure what these addresses are about but they could be the IR signals corresponding to an input ID.
        toggleRelayState(relayId);
        
        Serial.println("results.value after update: " + results.value , HEX);
        
        irrecv.resume();
    }
}

void checkButtons() {
    for (int i = 0; i < sizeof(buttonPins); i++) {
        // read the state of the pushbutton value:
        buttonState = digitalRead(buttonPins[i]);
  
        // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
        if (buttonState == HIGH) {
            toggleRelayState(i);
        }
    }
}

void toggleRelayState(int relayId) {
    // Toggle Relay ON or OFF
    if (relayStatusMap[relayId] == 0) {
        digitalWrite(updatePinColour, HIGH);
        relayStatusMap[relayId] = 1;
    } else {
        digitalWrite(updatePinColour, LOW);
        relayStatusMap[relayId] = 0;
    }
}

// This method should update the LCD display to output whatever the current state of the relays is
void updateLcdDisplay() {
    lcd.clear();
    for (int i=0; i<sizeof(relayStatusMap); i++) {
        lcd.setCursor(0, i); // 0th index, row 0
        String status = relayStatusMap[i] == 0 ? "ON" : "OFF";
        lcd.print("Relay " + i + " - " + status);
    }
}
