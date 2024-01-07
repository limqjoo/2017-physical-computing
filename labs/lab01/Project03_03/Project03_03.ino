//This code adjusts the brightness of an led light according to the temperature
//The LED is connected to a PWM pin
//Some averaging is done otherwise the led light will flicker annoyingly

const int sensorPin = A0; //connects to temperature sensor
const int ledPin = 3; //PWM pin connects to positive of LED
const float baselineTemp = 17.0;
float temperature = 17.0;
float prevTemp = 17.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //open a serial port

  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = analogRead(sensorPin);

  Serial.print("Sensor Value: ");
  Serial.println(sensorVal);

  //convert ADC reading to voltage
  float voltage = (sensorVal / 1024.0) * 5.0;

  Serial.print(", Volts: ");
  Serial.println(voltage);

  Serial.print(", degrees C: ");
  //convert voltage to temperature in degrees
  prevTemp = temperature;
  temperature += (((voltage - .5) * 100) - prevTemp) * 0.05 ; //doing some averaging because the values are erratic
  Serial.println(temperature);

  if (temperature > baselineTemp){
    analogWrite(ledPin, map(temperature, baselineTemp, baselineTemp + 4, 10, 255)); //map values
  }
  
  delay(10);
}
