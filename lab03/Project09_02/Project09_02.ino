// named constants for the switch and motor pins
const int upPin = 2; // the number of the speed increase pin
const int downPin = 3; // the number of the speed increase pin
const int motorPin =  9; // the number of the motor pin

int upState = 0;  // variable for reading the switch's status
int downState = 0;  // variable for reading the switch's status

int motorSpeed = 0;

void setup() {
  Serial.begin(9600);
  // initialize the motor pin as an output:
  pinMode(motorPin, OUTPUT);
  // initialize the switch pin as an input:
  pinMode(upPin, INPUT);
    pinMode(downPin, INPUT);
}

void loop() {
  // read the state of the switch value:
  upState = digitalRead(upPin);
  downState = digitalRead(downPin);

  // check if the switch is pressed.
  if (upState != downState) {
    if (upState == HIGH) {
      //increase speed of motor
      motorSpeed += 5;
    } else if (downState == HIGH) {
      motorSpeed -= 5;
    }
  }
  motorSpeed = constrain(motorSpeed, 0, 255);
  Serial.println(motorSpeed);
  analogWrite(motorPin, motorSpeed);
  
  delay(100);
}

