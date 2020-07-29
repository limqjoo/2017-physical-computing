/*
    Vibrating Diamonds
  Main program for Arduino UNO
  Created by Qian Joo, Lim
  7 DEC 2016
*/

#include "Manchester.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define RX_PIN 9 //receiver module
#define TX_PIN 8 //transmitter module
#define POT_PIN A0 //potentiometer
#define A_PIN 6 //ENTER button
#define B_PIN 7 //BACK button
#define COPPER_PIN A3 //copper strip
#define LED_PIN 1
#define LED_PIN2 13

#define BUFFER_SIZE 4
byte buffer[BUFFER_SIZE];

const byte ok = 90;

byte array_to_transmit[4] = {
  4, 65, 0, 0
};
const byte arraySize = 4;
byte received_array[4];
int identity = 'A';
int currentF = 0;
const int threshold = 300; //input from attiny copper tape
int prevStatus = 0;
int prevMode = 0;
int newF = 0;

const int frequencies[10] = {
  0, 55, 80, 105, 130, 155, 180, 205, 230, 255
}; //mapped to values for output to vibration motor: 71 - 80 (G - P)

/*
  modes:
  0 - no diamond docked
  1 - diamond docked, establishing 'connection' to diamond
  2 - menu (switch on/off or change frequency)
  3 - switch on/off
  4 - change frequency
  5 - exit to mode 2
*/
int mode = 0;


/*
  function: Sends tag within specified time
*/
void send_array(int to) {
  //digitalWrite(LED_PIN2, HIGH);
  Serial.println("sending...");
  //unsigned long timeout = millis() + to; //specifies how long to listen for array
  int moo = 0;
  while (moo < to) {
    man.transmitArray(arraySize, array_to_transmit);
    Serial.print("array sent: ");
    Serial.print(array_to_transmit[1]);
    Serial.print(array_to_transmit[2]);
    Serial.println(array_to_transmit[3]);
    delay(50); //space out the signals for less noise, and to make sure it doesn't pick up it's own signal at the end
    moo++;
  }
  delay(100);
}

/*
  function: Receives tag within specified time
*/
boolean receiving_array(int to) {
  Serial.println("receiving...");
  unsigned long timeout = millis() + to; //specifies how long to listen for array
  while (millis() < timeout) {
    if (man.receiveComplete())
    {
      if (buffer[0] == arraySize) { //if received array size matches
        Serial.print(checksum(buffer[1], buffer[2]));
        if (checksum(buffer[1], buffer[2]) == buffer[3]) { //if checksum matches
          Serial.println(" checksum ok");
          for (byte i = 0; i < arraySize; i++) {
            received_array[i] = buffer[i]; //save received array to local memory
            Serial.println(received_array[i]);
          }
          man.beginReceiveArray(BUFFER_SIZE, buffer);

          return true; //exit function with saved received array
        }
      }
    }
  }
  return false;
}

/*
   function: Receivies an expected specific tag (only middle two elements) within specified time
*/
boolean receiving_array(int to, byte* expected_array) {
  Serial.println("receiving...");
  unsigned long timeout = millis() + to; //specifies how long to listen for array
  while (millis() < timeout) {
    if (man.receiveComplete())
    {
      if (buffer[0] == arraySize) { //if received array size matches
        Serial.print(buffer[1]);
        Serial.println(buffer[2]);
        if (expected_array[0] == buffer[1] && expected_array[1] == buffer[2]) { //if tags match
          Serial.print(checksum(buffer[1], buffer[2]));
          if (checksum(buffer[1], buffer[2]) == buffer[3]) { //if checksum matches
            Serial.println(" checksum ok");
            for (byte i = 0; i < arraySize; i++) {
              received_array[i] = buffer[i]; //save received array to local memory
              Serial.println(received_array[i]);
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
int checksum(int a, int b) {
  int cs = (a * b * 13) % 8 + 82;
  return cs;
}

/*
  function: Checksum generator for sending arrays
*/
int checksum() {
  return checksum(array_to_transmit[1], array_to_transmit[2]);
}

//-------------------------------------------------------------------
void setup()
{
  pinMode(POT_PIN, INPUT);
  pinMode(COPPER_PIN, INPUT);
  pinMode(A_PIN, INPUT);
  pinMode(B_PIN, INPUT);
  Serial.begin(9600);
  man.setupReceive(RX_PIN, MAN_300);
  man.setupTransmit(TX_PIN, MAN_300);
  man.beginReceiveArray(BUFFER_SIZE, buffer);
  //Serial.println("going to mode 0");
  lcd.begin(16, 2);
  lcd.print("Initialised.");
}

void loop()
{
  if (mode != 0 && analogRead(COPPER_PIN) > threshold){ //if nothing is on dock, input from copper tape is HIGH
    //clear variables & go to mode 0
    identity = 0;
    currentF = 0;
    mode = 0;
    }
    /*
    MODE 0 - No diamond docked
  */
  if (mode == 0) {
    //display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nothing docked.");
    lcd.setCursor(0, 1);
    lcd.print("Ready.");
    if (analogRead(COPPER_PIN) < threshold) { //if diamond is docked, input from copper tape is LOW
      mode = 1;
    }
  }
  /*
    MODE 1 - Diamond detected in dock, establishing 'connection' to diamond
  */
  else if (mode == 1) {
    //display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Diamond docked.");
    lcd.setCursor(0, 1);
    lcd.print("Connecting...");
    //receive tag from diamond
    if (receiving_array(10000)) { //10 seconds, looped infinitely
      //if received array from docked diamond
      //get identity and current frequecny of diamond
      identity = received_array[1];
      currentF = received_array[2];

      //send affirmative to diamond so diamond can switch to receiving
      array_to_transmit[1] = identity;
      array_to_transmit[2] = ok;
      array_to_transmit[3] = checksum();
      send_array(5);

      //diplay
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Connected!");
      //Serial.println("going to mode 2");

      mode = 2;
    }
  }
  /*
     MODE 2 -Diamond in dock (choose action)
  */
  else if (mode == 2) {
    //read value from potentiometer, map to 0 or 1
    Serial.println(analogRead(POT_PIN));
    int v = map(analogRead(POT_PIN), 0, 1023, 0, 1);
    //display
    if (prevStatus == 0) { //prev display not the same)
      lcd.clear();
      if (v == 0) { // first option selected
        lcd.setCursor(0, 0);
        lcd.print(">    ON/OFF    <");
        lcd.setCursor(0, 1);
        lcd.print("  CHANGE FREQ.  ");
      } else if (v == 1) { //second option selected
        lcd.setCursor(0, 0);
        lcd.print("     ON/OFF     ");
        lcd.setCursor(0, 1);
        lcd.print("> CHANGE FREQ. <");
      }
      prevStatus = 1;
    }
    if (digitalRead(A_PIN) == HIGH && digitalRead(B_PIN) == LOW) { //ENTER button pressed
      prevStatus = 0;
      mode = 3 + v;
    }
  }
  /*
    MODE 3 - Diamond in dock - turn diamond on/off
  */
  else if (mode == 3) {
    //display
    if (prevStatus == 0) { //prev display not the same)
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Press ENTER to");
      lcd.setCursor(0, 1);
      lcd.print("switch on/off.");
      prevStatus = 1;
    }
    if (digitalRead(A_PIN) == HIGH && digitalRead(B_PIN) == LOW) { //ENTER button pressed
      array_to_transmit[2] = 81;
      array_to_transmit[3] = checksum();
    } else if (digitalRead(A_PIN) == LOW && digitalRead(B_PIN) == HIGH) { // BACK button pressed
      prevStatus = 0;
      prevMode = 3;
      mode = 5;
    }
  }

  /*
     MODE 4 - Diamond in dock - change frequency of diamond
  */
  else if (mode == 4) {
    //read value from potentiometer
    //int newF = map(analogRead(POT_PIN), 0, 1023, 0, 9);

    //display
    if (prevStatus == 0) { //prev display not the same)
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Current:  New:");
      lcd.setCursor(0, 1);
      lcd.print(frequencies[currentF - 70]);
      lcd.setCursor(10, 1);
      lcd.print(frequencies[newF - 70]);
      prevStatus = 1;
    }

    if (digitalRead(A_PIN) == HIGH) { //if user presses ENTER button
      Serial.println("A");
      prevStatus = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sending new fq...");
      lcd.setCursor(0, 1);
      lcd.print("Please wait...");

      //send new frequency to diamond
      array_to_transmit[2] = newF + 70;
      newF++;
      array_to_transmit[3] = checksum();
      send_array(1);

      byte expected_tag[2] = {array_to_transmit[1], ok};
      if (receiving_array(1000, expected_tag)) { //if received affirmative (that diamond received new frequency)
        currentF = newF;
        //display
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Current fq: ");
        lcd.setCursor(12, 0);
        lcd.print(frequencies[currentF - 70]);
        lcd.setCursor(0, 1);
        lcd.print("New fq set!");
        Serial.println("success");
        delay(1000);
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sending new fq...");
        lcd.setCursor(0, 1);
        lcd.print("Failed to send.");
        Serial.println("fail");
        delay(1000);
      }
    }
    /*else if (digitalRead(A_PIN) == LOW && digitalRead(B_PIN) == HIGH) { //if user presses back button
      Serial.println("B");
      prevMode = 4;
      prevStatus = 0;
      mode = 5;
      }*/
  }
  /*
     MODE 5 - Diamond in dock - exit screen to mode 3
  */
  else if (mode == 5) {
    //display
    if (prevStatus == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Go back to");
      lcd.setCursor(0, 1);
      lcd.print("previous menu?");
      prevStatus = 1;
    }
    if (digitalRead(A_PIN) == HIGH && digitalRead(B_PIN) == LOW) { //pressed enter button - exit
      //exit to mode 3
      mode = 3;
    }
    else if (digitalRead(A_PIN) == LOW && digitalRead(B_PIN) == HIGH) { //pressed back button - continue in previousMode
      prevStatus = 0;
      mode = prevMode;
    }
  }
}

