import ddf.minim.*;
import ddf.minim.ugens.*;

Minim       minim;
AudioOutput out;
Oscil       wave;

import processing.serial.*;

Serial myPort;
String data;
float amp = 0, freq = 0;
int form = 3; 

void setup() {
  size(512, 200, P3D);

  minim = new Minim(this);

  // use the getLineOut method of the Minim object to get an AudioOutput object
  out = minim.getLineOut();

  // create a sine wave Oscil, set to 440 Hz, at 0.5 amplitude
  wave = new Oscil( 440, 0.5f, Waves.SINE );
  // patch the Oscil to the output
  wave.patch( out );

  myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  if ( myPort.available() > 0) {
    // read the value and store it in a variable
    data = myPort.readStringUntil('.');
    if (data != null) {
      float[] values = float(split(data, ','));
      println(values);
      if (values.length == 3) {
        if (amp != values[0] && values[0] <= 1023) {
          amp = values[0];
          wave.setAmplitude( map( values[0], 0, 1023, 1, 0 ));
        }
        if (freq != values[1] && values[1] <= 1023) {
          freq = values[1];
          wave.setFrequency( map( values[1], 0, 1023, 100, 1000 ));
        }
        if (values[2] == 1.0) {
          println("change wave");
          form++;
          form%=5;
          if (form == 0) { 
            wave.setWaveform( Waves.SINE );
          } else if (form == 1) {

            wave.setWaveform( Waves.TRIANGLE );
          } else if (form == 2) {
            wave.setWaveform( Waves.SAW );
          } else if (form == 3) {
            wave.setWaveform( Waves.SQUARE );
          } else if (form == 4) {
            wave.setWaveform( Waves.QUARTERPULSE );
          }
        }
      }
    }

    background(0);
    stroke(255);
    strokeWeight(1);

    // draw the waveform of the output
    for (int i = 0; i < out.bufferSize() - 1; i++)
    {
      line( i, 50  - out.left.get(i)*50, i+1, 50  - out.left.get(i+1)*50 );
      line( i, 150 - out.right.get(i)*50, i+1, 150 - out.right.get(i+1)*50 );
    }

    // draw the waveform we are using in the oscillator
    stroke( 128, 0, 0 );
    strokeWeight(4);
    for ( int i = 0; i < width-1; ++i )
    {
      point( i, height/2 - (height*0.49) * wave.getWaveform().value( (float)i / width ) );
    }
  }
}