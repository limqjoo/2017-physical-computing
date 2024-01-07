void setup() {
  // initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // read the value of A0, divide by 4 and 
  // send it as a byte over the serial connection
  Serial.write(map(analogRead(A0), 0, 1023, 0, 255));
  //Serial.println(analogRead(A0));
  delay(100);
}



