//This code is for an interactive experiment with a series of LED lights, which is an expansion of Project 03_01
//There are two sensors, one controlling three LEDs each.
//The middle LED (controlled by pin 5) only lights up when LEDs on both sides are all lighted up already.

const int sensorPin1 = A0;
const int sensorPin2 = A1;
const float baselineTemp = 18.5;

void setup() {
  Serial.begin(9600); //open a serial port

  for (int pinNumber = 2; pinNumber < 9; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
}

void loop() {
  int sensorVal1 = analogRead(sensorPin1);
  int sensorVal2 = analogRead(sensorPin2);

  if (controlLights(sensorVal1, 2, 3, 4) && controlLights(sensorVal2, 6, 7, 8)){ //all led lights are lighted
    digitalWrite (5, HIGH); //middle LED lights up
  } else {
    digitalWrite (5, LOW); //middle LED does not light up
  }

  delay(1);
}

float calculateTemp(int sensorVal) {
  //calculates temperature from sensor value
  
  Serial.print("Sensor Value: ");
  Serial.println(sensorVal);

  //convert ADC reading to voltage
  float voltage = (sensorVal / 1024.0) * 5.0;

  Serial.print(", degrees C: ");
  //convert voltage to temperature in degrees
  float temperature = (voltage - .5) * 100;
  Serial.println(temperature);

  return temperature;
}

bool controlLights(int sensorVal, int LED1, int LED2, int LED3) {
  //designates number of led lights to be lighted up
  //returns true if all lights are lighted
  
  float temperature = calculateTemp(sensorVal);

  if (temperature < baselineTemp) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    
    return false;
  } else if (temperature >= baselineTemp && temperature < baselineTemp + 0.5) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    
    return false;
  } else if (temperature >= baselineTemp + 0.5 && temperature < baselineTemp + 1) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    
    return false;
  } else if (temperature >= baselineTemp + 1) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);

    return true;
  }
}

