#include <LiquidCrystal_I2C.h>

/*
  IR Receiver Demonstration 3
  IR-Rcv-Demo3.ino
  Control LED's using Unused IR Remote keys
 
  DroneBot Workshop 2017
  http://dronebotworkshop.com
*/
 
// Include IR Remote Library by Ken Shirriff
#include <IRremote.h>
 
// Define sensor pin
const int RECV_PIN = 4;
 
// Define LED pin constants
const int redPin = 5; 
const int yellowPin = 6;
const int bluePin = 7;
const int greenPin = 8;
const int pinkPin = 9;
const int brownPin = 10;
const int violetPin = 11;
const int greyPin = 12;







int inPin = 15;         // the number of the input pin
int outPin = 16;       // the number of the output pin

int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long time = 0;           // the last time the output pin was toggled
unsigned long debounce = 200UL;   // the debounce time, increase if the output flickers


 
// Define integer to remember toggle state
int togglestate = 0;
 
// Define IR Receiver and Results Objects
IRrecv irrecv(RECV_PIN);
decode_results results;
 
 
void setup(){

  {
  pinMode(inPin,  INPUT);
  pinMode(outPin, OUTPUT);
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

}
 
 
void loop(){

  {
  reading = digitalRead(inPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce)
  {
    if (state == HIGH)
      state = LOW;
    else
      state = HIGH;
     

    time = millis();
  }

  digitalWrite(outPin, state);

  //previous = reading;
 
}

    if (irrecv.decode(&results)){

      // Serial Monitor @ 9600 baud
          Serial.println(results.value, HEX);

      switch(results.value){

        
          
          case 0xA25D807F: //Red Keypad Button
        // Toggle LED On or Off
        if(togglestate==0){
        digitalWrite(redPin, LOW);
        togglestate=1;
        }
        else {
        digitalWrite(redPin, HIGH);
        togglestate=0;
        }
        break;
            
          case 0xA25D40BF: //Yellow Keypad Button
        // Toggle LED On or Off
        if(togglestate==0){
        digitalWrite(yellowPin, LOW);
        togglestate=1;
        }
        else {
        digitalWrite(yellowPin, HIGH);
        togglestate=0;
        }
        break;

        case 0xA25DC03F: //Blue Keypad Button
        // Toggle LED On or Off
        if(togglestate==0){
        digitalWrite(bluePin, LOW);
        togglestate=1;
        }
        else {
        digitalWrite(bluePin, HIGH);
        togglestate=0;
        }
        break;

        case 0xA25D20DF: //Green Keypad Button
        // Toggle LED On or Off
        if(togglestate==0){
        digitalWrite(greenPin, LOW);
        togglestate=1;
        }
        else {
        digitalWrite(greenPin, HIGH);
        togglestate=0;
        }
        break;

        case 0xA25DA05F: //Pink Keypad Button
        // Toggle LED On or Off
        if(togglestate==0){
        digitalWrite(pinkPin, LOW);
        togglestate=1;
        }
        else {
        digitalWrite(pinkPin, HIGH);
        togglestate=0;
        }
        break;

        case 0xA25D609F: //Brown Keypad Button
        // Toggle LED On or Off
        if(togglestate==0){
        digitalWrite(brownPin, LOW);
        togglestate=1;
        }
        else {
        digitalWrite(brownPin, HIGH);
        togglestate=0;
        }
        break;

        

         case 0xA25DE01F: //Violet Keypad Button
        // Toggle LED On or Off
        if(togglestate==0){
        digitalWrite(violetPin, LOW);
        togglestate=1;
        }
        else {
        digitalWrite(violetPin, HIGH);
        togglestate=0;
        }
        break;

                case 0xA25D10EF: //Grey Keypad Button
        // Toggle LED On or Off
        if(togglestate==0){
        digitalWrite(greyPin, LOW);
        togglestate=1;
        }
        else {
        digitalWrite(greyPin, HIGH);
        togglestate=0;
        }
        break;

               
     Serial.println(results.value);    
    }
    irrecv.resume(); 
  }
 
}
