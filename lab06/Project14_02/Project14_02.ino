void setup() {
  // initialize serial communication
  Serial.begin(9600);
  
  pinMode(2, INPUT);
}

void writeString(String stringData) { // Used to serially push out a String with Serial.write()

  for (int i = 0; i < stringData.length(); i++)
  {
    Serial.write(stringData[i]);   // Push each char 1 by 1 on each loop pass
  }

}// end writeString

void loop() {
  String data = String(analogRead(A0))+","+String(analogRead(A1))+","+String(digitalRead(2))+".";
  writeString(data);
  //Serial.write(data);
  delay(100);
}



