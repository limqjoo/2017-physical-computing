// named constant for the switch pin
const int switchPin = 2;

unsigned long previousTime = 0; // store the last time an LED was updated
int switchState = 0; // the current switch state
int prevSwitchState = 0; // the previous switch state
int led = 8; // a variable to refer to the LEDs

// 600000 = 10 minutes in milliseconds
long interval = 1000; // interval at which to light the next LED 

void setup() {
  // set the LED pins as outputs
  for(int x = 8;x<14;x++){
    pinMode(x, OUTPUT);
  }
  // set the tilt switch pin as input 
  pinMode(switchPin, INPUT);
}

void loop(){ 
  // store the time since the Arduino started running in a variable 
  unsigned long currentTime = millis(); 

  // compare the current time to the previous time an LED turned on
  // if it is greater than your interval, run the if statement
  if(currentTime - previousTime > interval) {
    // save the current time as the last time you changed an LED 
    previousTime = currentTime; 
    // Turn the LED on
    digitalWrite(led, HIGH);
    // increment the led variable
    // in 10 minutes the next LED will light up   
    led++; 
    
    if(led == 13){
      // the hour is up
    }
  }

  // read the switch value
  switchState = digitalRead(switchPin); 
  
  // if the switch has changed
  if(switchState != prevSwitchState){
    // turn all the LEDs low
    for(int x = 8;x<14;x++){    
      digitalWrite(x, LOW);
    }  
    
    // reset the LED variable to the first one
    led = 8;
    
    //reset the timer
    previousTime = currentTime;
  }
  // set the previous switch state to the current state
  prevSwitchState = switchState;
}

