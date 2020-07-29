// create an array of notes
// the numbers below correspond to
// the frequencies of C4 to C5.
int notes[] = {262, 294, 330, 370, 392, 440, 494, 523, 587};
int keyboard = 0;

void setup() {
  //start serial communication
  Serial.begin(9600);
  //pinMode(7, INPUT);
}

void loop() {
  // create a local variable to hold the input on pin A0
  int keyVal = analogRead(A0);
  int lightVal = analogRead(A1);

  Serial.println(lightVal);

  if (lightVal > 700) {
    keyboard = 0;
  }
  else if (lightVal > 300) {
    keyboard = 3;
  } else {
    keyboard = 6;
  }
  
  // send the value from A0 to the Serial Monitor
  Serial.println(keyVal);

  // play the note corresponding to each value on A0
  if (keyVal == 1023) {
    // play the first frequency in the array on pin 8
    tone(8, notes[0 + keyboard]);
  }
  else if (keyVal >= 990 && keyVal <= 1010) {
    // play the second frequency in the array on pin 8
    tone(8, notes[1 + keyboard]);
  }
  else if (keyVal >= 505 && keyVal <= 515) {
    // play the third frequency in the array on pin 8
    tone(8, notes[2 + keyboard]);
  }
  else {
    // if the value is out of range, play no tone
    noTone(8);
  }
}
