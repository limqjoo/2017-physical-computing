//alarm/timer

// named constant for the switch pin
const int switchPin = 2;
const int piezoPin = 5;

unsigned long currentTime, timer = 5000;
unsigned long prevTime = 0; // store the last time an LED was updated
int switchState = 0; // the current switch state
int prevSwitchState = 0; // the previous switch state
int frequency, snoozed;

void setup() {
  Serial.begin(9600);
  // set the tilt switch pin as input
  pinMode(switchPin, INPUT);

  // store the time since the Arduino started running in a variable
  prevTime = millis();
}

void loop() {
  currentTime = millis();
  switchState = digitalRead(switchPin);
  if (currentTime - prevTime > timer) {
    //sounds the alarm
    frequency+=110;
    frequency%=550;
    tone(piezoPin, frequency + snoozed*110, 250);
  }
  if (switchState != prevSwitchState){
    //tilt switch is toggled (snooze)
    prevTime = currentTime; //reset timer
    prevSwitchState = switchState;
    snoozed++;
  }
  delay(100);
}
