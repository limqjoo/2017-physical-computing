//This code is for controlling the colour of a RGB led with two photoresistors.
//If the light source is nearer (or stronger) to the left resistor, the led light will appear redder in colour,
//conversely, the led will appear more blue in colour if the light is stronger to the right resistor.

const int greenLEDPin = 9;
const int blueLEDPin = 10;
const int redLEDPin = 11;

//useful constants
const int lSensorPin = A0;
const int rSensorPin = A1;

int lValue = 0;
int rValue = 0;

//variables to store the sensor readings as well as the light level of each LED
int lSensorValue = 0;
int rSensorValue = 0;

void setup() {
  Serial.begin(9600);
  //setting the direction of the digital pins and setting up the serial port
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
}

void loop() {
  //reading the vlaue of each light sensor
  lSensorValue = analogRead(lSensorPin);
  delay(5);
  rSensorValue = analogRead(rSensorPin);

  //Convert the sensor readings from a value between 0 - 1023
  //to a value between 0 - 255
  int prevLValue = lValue;
  int prevRValue = rValue;
  lValue += ((lSensorValue / 4) - prevLValue) * 0.1; //I have done some averaging to prevent the values from being too erratic.
  rValue += ((rSensorValue / 4) - prevRValue) * 0.1;

  Serial.print("Mapped Sensor Values \t L:");
  Serial.print(lValue);
  Serial.print(" \t R:");      //report the calculated LED light levels
  Serial.println(rValue);
  
  //Set the LED light levels
  //values are mapped to ratio of red and blue light proportions
  analogWrite(redLEDPin, map(lValue, 0, lValue + rValue, 0, 255));
  analogWrite(greenLEDPin, 0);
  analogWrite(blueLEDPin, map(rValue, 0, lValue + rValue, 0, 255));
}
