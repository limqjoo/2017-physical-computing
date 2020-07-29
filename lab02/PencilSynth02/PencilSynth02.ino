int value;
int piezoPin = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(piezoPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(0);

  Serial.print("Raw Value:");
  Serial.println(value);
  
  int note = map(value, 0, 1023, 0, 16);
  float freq = 440 * pow(1.059463094359, note); //calculate frequency for the note
  tone(piezoPin, freq);

  delay(100);
}
