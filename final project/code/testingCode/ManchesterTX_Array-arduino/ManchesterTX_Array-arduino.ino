
#include <Manchester.h>

/*

 Manchester Transmitter example
 
 In this example transmitter will send 10 bytes array  per transmittion
 
 try different speeds using this constants, your maximum possible speed will 
 depend on various factors like transmitter type, distance, microcontroller speed, ...
 
 MAN_300 0
 MAN_600 1
 MAN_1200 2
 MAN_2400 3
 MAN_4800 4
 MAN_9600 5
 MAN_19200 6
 MAN_38400 7
 
 */

#define TX_PIN  4  //pin where your transmitter is connected
#define LED_PIN 13 //pin for blinking LED

uint8_t moo = 1; //last led status
//uint8_t data[20] = {11, '1','2', '3', '4', '5', '6', '7', '8', '9','1','2','3','4','5','6','7','8','9'};
uint8_t data[20] = {
  11, 'A','B', 'C', '4', '5', '6', '7', '8', '9','1','2','3','4','5','6','7','8','9'};

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, moo);
  man.setupTransmit(TX_PIN, MAN_4800);
}

uint8_t datalength=3;

void loop() 
{
  data[0] = datalength;

  man.transmitArray(datalength, data);
  moo = ++moo % 2;
  digitalWrite(LED_PIN, moo);

  delay(800);
}

