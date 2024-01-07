//a really weird tone generator
//using a tilt switch
//the faster you shake the tllt switch (to break and connect the circuit)
//the higher the tone produced from the piezo

// named constant for the switch pin
const int switchPin = 2;
const int piezoPin = 5;

unsigned long currentTime, interval = 3000, prevInterval = 3000;
unsigned long prevTime = 0; // store the last time an LED was updated
int switchState = 0; // the current switch state
int prevSwitchState = 0; // the previous switch state
float frequency;

void setup() {
  Serial.begin(9600);
  // set the tilt switch pin as input
  pinMode(switchPin, INPUT);

  // store the time since the Arduino started running in a variable
  currentTime = millis();
  prevTime = currentTime;
}

void loop() {
  currentTime = millis();
  // read the switch value
  prevSwitchState = switchState;
  switchState = digitalRead(switchPin);
  Serial.println(interval);
  if (prevSwitchState == LOW && switchState == HIGH) { //ball bearing in contact again after disconnecting the switch
    //    unsigned long interval = currentTime - prevTime;
    //interval = min(currentTime - prevTime, 3000); //get interval between two flicks (max. 3 seconds)
    //the longer the interval, the lower the frequency
    prevInterval = interval;
    interval += (interval - min(currentTime - prevTime, 3000)) * 0.1; //averaging
    prevTime = currentTime;
  } else if (prevSwitchState == HIGH && switchState == HIGH) {
    interval += 1;
    interval = min(3000, frequency); //choose the least value from 3000 or frequency
  }
  //Serial.println(interval);
  frequency = map(interval, 6, 3000, 1000, 60);
//  Serial.println(frequency);
  tone(piezoPin, frequency);

  //Serial.println(currentTime-prevTime);
}
