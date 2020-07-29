//This code does exactly as detailed in Project 03 of the Arduino Projects Book.

const int sensorPin = A0;
const float baselineTemp = 19.5; //adjusted temperature for surroundings and my cold fingers.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //open a serial port

  for (int pinNumber = 2; pinNumber < 5; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
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
  float temperature = (voltage - .5) * 100;
  Serial.println(temperature);

  if (temperature < baselineTemp) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  } else if (temperature >= baselineTemp && temperature < baselineTemp + 0.5) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  } else if (temperature >= baselineTemp + 0.5 && temperature < baselineTemp + 1) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  } else if (temperature >= baselineTemp + 1) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
  }
  delay(1);
}
