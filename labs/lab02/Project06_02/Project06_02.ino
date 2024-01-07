//This code is for controlling the pitch of piezo element with light sensor.
//The brighter it is, the higher the pitch.

//useful constants
const int piezoPin = 11;
const int lightSensorPin = A0;
int value = 0;

//variables to store the sensor readings as well as the light level of each LED
int sensorValue = 0;

void setup() {
  Serial.begin(9600);
  //setting the direction of the digital pins and setting up the serial port
  pinMode(11, OUTPUT);
}

void loop() {
  //reading the vlaue of each light sensor
  sensorValue = analogRead(lightSensorPin);

  Serial.print("Raw Sensor Value:");
  Serial.println(sensorValue);

  //Convert the sensor readings from a value between 0 - 1023
  //to a value between 0 - 255
  int prevValue = value;
  value += ((sensorValue / 4) - prevValue) * 0.1; //I have done some averaging to prevent the values from being too erratic.

  Serial.print("Mapped Sensor Value:");
  Serial.println(value);

  int note = map(sensorValue, 0, 255, 0, 16);
  float freq = 440 * pow(1.059463094359, note); //calculate frequency for the note
  tone(piezoPin, freq);
  delay(250);
}
