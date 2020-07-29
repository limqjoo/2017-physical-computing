int switchState = 0;
int lightState = 0;
unsigned long downTime = 0; 
unsigned long upTime = 0;
unsigned long startPress = 0; 


void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  switchState = digitalRead(2);

  if (switchState == LOW && startPress > 0){
    //button just released
    upTime = millis(); //record time when the button is released
    downTime = upTime - startPress; //duration of button press
    startPress = 0; //reset startPress
  } else if (switchState == HIGH && startPress == 0) {
    //button just pressed
    startPress = millis(); //record time when the button when pressed
  }

  if (downTime > 2000){
    //if button pressed for more than 2 seconds
    lightState = 1;
    downTime = 0; //reset downTime
  } else if (downTime > 0){
    //if button pressed for less than 2 seconds
    lightState = 2;
    downTime = 0; // reset downTime
  } else if (upTime != 0 && (millis() - upTime) > 3000){
    //button not pressed for more than 3 seconds
    lightState = 0; //revert back to unlighted state
    upTime = 0; //reset upTime
  }

  if (lightState == 1){
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    delay(100);

    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    delay(100);
  } else if (lightState == 2) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    delay(200);

    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    delay(200);
  
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
    delay(200);
  } else { //lightState == 0
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
}

