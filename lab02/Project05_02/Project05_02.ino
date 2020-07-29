#include <Servo.h>

Servo myServo;

const int buttonPin1 = 4;
const int buttonPin2 = 5;

bool button1, button2;

int angle = 90;

void setup() {
  myServo.attach(9);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  Serial.begin(9600);
}

void loop() {
  if (digitalRead(buttonPin1) == HIGH){
    button1 = true;
  } else {
    button1 = false;
  }
  if (digitalRead(buttonPin2) == HIGH){
    button2 = true;
  } else {
    button2 = false;
  }
  if (button1 == HIGH && button2 == LOW) {
    //if button 1 is pressed and button 2 not pressed
    if (angle < 180) {
      angle ++;
    }
  } else if (button1 == LOW && button2 == HIGH) {
    //if button 2 is pressed and button 1 not pressed
    if (angle > 0) {
      angle--;
    }
    Serial.println(2);
  }

  Serial.print(", angle: ");
  Serial.println(angle);

  myServo.write(angle);
  delay(10);
}
