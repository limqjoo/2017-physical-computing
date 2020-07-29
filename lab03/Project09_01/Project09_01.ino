// named constants for the switch and motor pins
const int switchPin = 2; // the number of the switch pin
const int motorPin =  9; // the number of the motor pin

int switchState = 0;  // variable for reading the switch's status

void setup() {
  Serial.begin(9600);
  // initialize the motor pin as an output:
  pinMode(motorPin, OUTPUT);      
  // initialize the switch pin as an input:
  pinMode(switchPin, INPUT);     
}

void loop(){
  // read the state of the switch value:
  switchState = digitalRead(switchPin);

  // check if the switch is pressed.
  if (switchState == HIGH) {     
    // turn motor on:    
    analogWrite(motorPin, 255);  
    Serial.println('on');
  } 
  else {
    // turn motor off:
    analogWrite(motorPin, 0); 
    Serial.println('off');
  }
}

