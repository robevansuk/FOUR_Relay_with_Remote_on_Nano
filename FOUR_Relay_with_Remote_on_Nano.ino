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

//---------------- 
// mutable state
//----------------
int outputState                = HIGH;      // the current state of the output pin
int reading;                                // the current reading from the input pin
int previousReading            = LOW;       // the previous reading from the input pin  

// the following variables are long's because the time, measured in miliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastUpdateTime   = 0;          // the last time the output pin was toggled
unsigned long debounce         = 200UL;      // the debounce time, increase if the output flickers

// Define integer to remember toggle state
int togglestate                = 0;

// Define IR Receiver and Results Objects
IRrecv irrecv(RECV_PIN);
decode_results results;

// Set the LCD address to 0x27 (not sure what this is) for a 16 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 16, 4);

const map<int, bool> relayStatusMap;
relayStatusMap[1] = true;
relayStatusMap[2] = true;
relayStatusMap[3] = true;
relayStatusMap[4] = true;

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

const map<int, int> pinAddressMap;
pinAddressMap[0xA25D807F] = redPin;
pinAddressMap[0xA25D40BF] = yellowPin;
pinAddressMap[0xA25DC03F] = bluePin;
pinAddressMap[0xA25D20DF] = greenPin;
pinAddressMap[0xA25DA05F] = pinkPin;
pinAddressMap[0xA25D609F] = brownPin;
pinAddressMap[0xA25DE01F] = violetPin;
pinAddressMap[0xA25D10EF] = greyPin;

void loop() {
  
    {
        reading = digitalRead(inPin); // read state from pin
        
        // if the input just went from LOW to HIGH and we've waited long enough
        // to ignore any noise on the circuit, toggle the output pin
        // Millis - time since start of program. Resets after ~50 days due to overflow. Indicates a bug here unless device auto restarts every 50 days to mitigate. Or some other maths occurs to mitigate.
        // If we get reading == HIGH && previousReading == HIGH - there could be a longer than standard delay here if debounce is more than 100-150 since there'll need to be at least two cycles to allow variables to get back to steady state.
        if (reading == HIGH && previousReading == LOW && millis() - lastUpdateTime > debounce) {
            toggleOutput();
            lastUpdateTime = millis();
        }

        digitalWrite(outPin, outputState); // update state

        previousReading = reading;
    }

    if (irrecv.decode(&results)) {

        // Serial Monitor @ 9600 baud
        Serial.println("results.value before update : " + results.value, HEX);
        
        updateState(togglestate, pinAddressMap[results.value]);
        
        Serial.println("results.value after update: " + results.value , HEX);
        
        irrecv.resume();
    }

    updateLcdDisplay();

}

void toggleOutput() {
  if (outputState == HIGH) {
      outputState = LOW;
  } else {
      outputState = HIGH;
  }
}

void updateState(int currentState, int updatePinColour) {
    // Toggle LED On or Off
    if (currentState == 0) {
        digitalWrite(updatePinColour, LOW);
        togglestate = 1;
    } else {
        digitalWrite(updatePinColour, HIGH);
        togglestate = 0;
    }
}

// This method should update the LCD display to output whatever the current state of the relays is
void updateLcdDisplay() {
  lcd.clear();
  lcd.setCursor(0,0); // 0th index, row 0
  lcd.print("Relay one - " + relayStatusMap[1]);
  lcd.setCursor(0,1); // 0th index, row 1
  lcd.print("Relay two - " + relayStatusMap[2]);
  lcd.setCursor(0,2); // 0th index, row 2
  lcd.print("Relay three - " + relayStatusMap[3]);
  lcd.setCursor(0,3); // 0th index, row 3
  lcd.print("Relay four - " + relayStatusMap[4]);
}
