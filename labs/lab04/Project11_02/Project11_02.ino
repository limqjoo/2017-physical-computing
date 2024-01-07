// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// set up a constant for the tilt switchPin
const int piezoPin = 9;

int song[] = {1, 1, 5, 5, 6, 6, 5, 0,
              4, 4, 3, 3, 2, 2, 1, 0,
              5, 5, 4, 4, 3, 3, 2, 0,
              5, 5, 4, 4, 3, 3, 2, 0,
              1, 1, 5, 5, 6, 6, 5, 0,
              4, 4, 3, 3, 2, 2, 1, 0
             };
int n = 0;

// create an array of notes
// the numbers below correspond to
// the frequencies of C4 to A4.
int notes[] = {262, 294, 330, 349, 392, 440};

int noteLen = 500; //how long each note lasts in milliseconds

// a variable to choose which reply from the crystal ball
int lyric = 0;

String lyrics[] = {"Twinkle twinkle little star",
                   "How I wonder what you are",
                   "Up above the world so high",
                   "Like a diamond in the sky",
                   "Twinkle twinkle little star",
                   "How I wonder what you are",
                  };

void setup() {

  // set up the number of columns and rows on the LCD
  lcd.begin(16, 2);
}

void loop() {
  //play the song
  if (song[n] != 0) {
    tone(piezoPin, notes[song[n] - 1], noteLen);
  }
  n++;
  n %= (int)sizeof(song) / sizeof(song[0]);

  //display lyrics
  lcd.clear(); //clear screen before printing new line
  lcd.setCursor(0, 0);  // set the cursor to column 0, line 0
  lcd.print(lyrics[lyric].substring(0, 16));
  // move the cursor to the second line
  lcd.setCursor(0, 1);
  lcd.print(lyrics[lyric].substring(16));

  if (n % 8 == 0) {
    lyric++;
    lyric %= (int)sizeof(lyrics) / sizeof(lyrics[0]);
  }
  delay(noteLen);
  if (n == 0) { //end of song
    //pause for while
    delay(2000);
  }
}


