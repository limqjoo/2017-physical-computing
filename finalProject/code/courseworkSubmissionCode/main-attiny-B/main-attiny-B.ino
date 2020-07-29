/*
    Vibrating Diamonds
 Main program for ATtiny85
 Created by Qian Joo, Lim
 7 DEC 2016
 */

#include "Manchester.h"

#define RX_PIN 0
#define TX_PIN 1
#define LED_PIN 3
#define VM_PIN 4 //vibration motor (does pwm really work?)

const byte identity = 65;
const int frequencies[10] = {//mapped to values for output to vibration motor: 70 - 80
  25, 30, 35, 40, 50, 60, 75, 85, 105, 125}; //values halved because motor is 3-4.5V, but power source is 6V
byte f = 71; //70-80

#define BUFFER_SIZE 4
byte buffer[BUFFER_SIZE];

byte array_to_transmit[4] = {
  4, identity, f, 0};
byte arraySize = 4;
byte received_array[4];

const byte ok = 90;

boolean on = true;

/*
function: Sends tag within specified time
 */
void send_array(int timeout){
  int time = 0;
  while (time < timeout){
    //digitalWrite(LED_PIN, HIGH);
    man.transmitArray(arraySize, array_to_transmit);
    delay(50);
    time++;
    //digitalWrite(LED_PIN, LOW);
  }
  delay(50); //to make sure it doesn't pick up it's own signal
  return;
}

/*
function: Receives tag within specified time, also checks received tag for matching id
 */
boolean receiving_array(int timeout){
  int  time = 0;
  while (time < timeout){
    if (man.receiveComplete()) 
    {
      if (buffer[0] == arraySize){
        if (checksum(buffer[1], buffer[2]) == buffer[3] && buffer[1] == identity){ //checksum and ID matches
          for (byte i = 0; i < arraySize; i++){
            received_array[i] = buffer[i];
          }
          man.beginReceiveArray(BUFFER_SIZE, buffer);
          return true;
        }
      }       
    }
    time++;
  }
  return false;
}

/*
   function: Receives expected specific within specified time
 */
boolean receiving_array(int timeout, byte a, byte b) {
  int  time = 0;
  while (time < timeout) {
    if (man.receiveComplete())
    {
analogWrite(VM_PIN, 120);
      if (buffer[0] == arraySize) { //if received array size matches
        if (buffer[1] == identity && buffer[2] >= a && buffer[2] <= b) { //if tags match
          if (checksum(buffer[1], buffer[2]) == buffer[3]) { //if checksum matches
            for (byte i = 0; i < arraySize; i++) {
              received_array[i] = buffer[i]; //save received array to local memory
            }
            man.beginReceiveArray(BUFFER_SIZE, buffer);

            return true; //exit function with saved received array
          }
        }
      }
    }
  }
  return false;
}

/*
    function: Calculates checksum
 */
int checksum(int a, int b){
  return (a*b*13)%8+82;
}

/*
function: Checksum generator for sending arrays
 */
int checksum(){
  return checksum(array_to_transmit[1], array_to_transmit[2]);
}

//-------------------------------------------------------------------
void setup() 
{
  pinMode(VM_PIN, OUTPUT);  
    pinMode(LED_PIN, OUTPUT);  
  man.setupReceive(RX_PIN, MAN_300);
  man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84 (lies. add this line even when it's 8Mhz)
  man.setupTransmit(TX_PIN, MAN_300);
  man.beginReceiveArray(BUFFER_SIZE, buffer);
  array_to_transmit[3] = checksum(identity, f);
}

void loop() 
{
          analogWrite(VM_PIN, 0);
 /* if (on){
    analogWrite(VM_PIN, 255);
  } 
  else {
    analogWrite(VM_PIN, 0);
  }*/
  if (receiving_array(10000, 70, 81)){
    if (received_array[2] >= 71 && received_array[2] <= 80){ //tag received contains frequency
      f = received_array[2]; //set new frequency
      analogWrite(VM_PIN, frequencies[f-71]);
    } 
    else if (received_array[2] == 81){ //tag received contains switch on/off instruction
      on = !on; //switch on/off
    }
    array_to_transmit[2] = ok; //set tag as affirmative
    array_to_transmit[3] = checksum();
    send_array(5); //send affirmative (confirms that valid tag received)
  }
}

