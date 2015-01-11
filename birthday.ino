#include <Tone.h>
#include <LiquidCrystal.h>

/* Happy birthday hack
*  Johan Euphrosine 2009
*  using freqout libray by Paul Badger
*  piezo on pin 9
*  push button on pin 8
*  leds on pin 10,11,12 for blinking madness
*/
/*  freqout(freq, t)  // freq in hz, t in ms
*   Paul Badger 2007
*   a simple tone generation function
*   generates square waves of arbitrary frequency and duration
*   program also includes a top-octave lookup table & transposition function
*/


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define  ON  HIGH
#define  OFF  LOW


char* words[5] = {"Happy", "Birthday", "To You", "To Dear <name>" , "From <yourname>"};
int  songSequences[5][3] = { {0,1,2}, {0,1,2},{0,1,3},{0,1,2}, {0,1,4}};


// For input/output set up
int speakerPin = 8;
int ledPins[]= { 5, 6 };
int inputPin = 10; //reed switch
int inputVal = 0;

// Calculate number of LEDs
int ledCount = sizeof(ledPins) / sizeof(int);

Tone makeTone;

// Song to play
int notes[] = { 
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4, 0, 
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4, 0,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4, 0,
  NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4, 0 }; // notes to play; see Tone.h for frequencies; 0 --> PAUSE
int songLength = sizeof(notes) / sizeof(int); // Calculate song length
int beats[] = { 
  1, 1, 2, 2, 2, 4, 2, 
  1, 1, 2, 2, 2, 4, 2, 
  1, 1, 2, 2, 2, 2, 4, 2, 
  1, 1, 2, 2, 2, 4, 4 }; // number of beats for each note
int numberBeats = sizeof(beats) / sizeof(int); // Calculate number of beats
int tempo = 200; // in milliseconds
// End song

void animateComponents(int note, int beat, int tempo, int leds[], int numberLeds) {
  makeTone.stop(); // speaker reset
  ledSwitch(leds, numberLeds, ON);
  makeTone.play(note); // play tone 
  delay(tempo * beat); // for specified number of beats
  makeTone.stop(); // speaker reset
  ledSwitch(leds, numberLeds, OFF);
  delay(tempo / 2); // pause between notes
}

void playPause(int beat, int tempo) {
  delay(tempo * beat); // pause for specified number of beats
  delay(tempo / 2); // pause between notes
}

void ledSwitch(int leds[], int numberLeds, int pinVoltage) {
  for (int i = 0; i < numberLeds; i++) {
    digitalWrite(leds[i], pinVoltage); // turn on/off LED
  }
}


void setup() { 
  for(int i = 0; i < ledCount; i++){
    pinMode(ledPins[i], OUTPUT); // set up LEDs for writing
  }
  pinMode(inputPin, INPUT); // set up reed switch
  makeTone.begin(speakerPin); // set up piezo speaker
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  
  
  lcd.blink();
  lcd.setCursor(0, 0);
  lcd.print("-- Getting ");
  lcd.setCursor(0, 1);
  lcd.print("Information --");
  delay(8000);
  lcd.clear();
  
  
  lcd.setCursor(0, 0);
  lcd.print("-- Connecting to");
  lcd.setCursor(0, 1);
  lcd.print("Google server --");
  delay(3000);
  lcd.clear();
  
  
  lcd.setCursor(0, 0);
  lcd.print("-- Scanning");
  lcd.setCursor(0, 1);
  lcd.print("Fingerprints --");
  delay(3000);
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("* Welcome <name> *");
  delay(3000);
  lcd.clear();
   lcd.noBlink();
  lcd.setCursor(0, 0);
  lcd.print("* Today is your *");
  lcd.setCursor(0, 1);
  lcd.print("    BIRTHDAY!");
  delay(3000);
  lcd.clear();
} 


void loop(){   
  lcd.setCursor(0, 0);
  lcd.print("Happy Birthday");
  lcd.setCursor(0, 1);
  lcd.print("Arduino max246");
  
  delay(6000);
  
  inputVal = digitalRead(inputPin); // default (not switched) = HIGH
  if (inputVal == LOW) { // do nothing until LOW, then play song
    if (songLength > 0 && numberBeats > 0 && tempo > 0 && songLength == numberBeats) { // Simple song error check
      for (int i = 0; i < songLength; i++) { // loop through song
        if (notes[i] == 0) { // PAUSE
          playPause(beats[i], tempo);
        }
        else { 
          Serial.print(beats[i]);
          Serial.print(" " );
          
          Serial.println(i);
          textToScreen(i,beats[i]);
          animateComponents(notes[i], beats[i], tempo, ledPins, ledCount); // make sound & light up LEDs
        }
        if (i == 28) {
          textToScreen(30,0);
          delay(4000);
        }
      }
    }
    else { // Play on song error
      makeTone.stop();
      makeTone.play(NOTE_C2);
      delay(1000);
      makeTone.play(NOTE_C1);
      delay(2000);
      makeTone.stop();
    }
  }
  
  
  
} 


void textToScreen(int i, int beat) {
    int index = 0;
    if (i >= 0 && i < 6) {
       index = 0;
      //Happy birthday to you
    } else if (i >=7 && i < 13) {
       index = 1;
      //Happy birthday to you
    } else if (i >= 14 && i < 21) {
       index = 2;
      //Happy birthday to Dear <name>
    } else if (i >= 22 && i < 28) {
       index = 3;
      //Happy birthday to you
    } else {
       index = 4;
      //From xxxx
    }
    lcd.clear();
    
    if (index != 4) {
        if (beat == 1) { //First beat, first part
            lcd.setCursor(0, 0);
            lcd.print(words[songSequences[index][0]]);
        } 
        
        else if (beat == 2) { //Medium beat, second part
            lcd.setCursor(0, 0); 
            lcd.print(words[songSequences[index][0]]);
            lcd.print(" ");
            lcd.print(words[songSequences[index][1]]);
        } 
        else if (beat == 4) { //Beat long so last part of the message
            lcd.setCursor(0, 0);
            lcd.print(words[songSequences[index][0]]);
            lcd.print(" ");
            lcd.print(words[songSequences[index][1]]);        
            lcd.setCursor(0, 1);
            lcd.print(words[songSequences[index][2]]);
        } 
    } else { //Song finished, show from who are these wishes
            lcd.setCursor(0, 0);
            lcd.print(words[songSequences[index][0]]);
            lcd.print(" ");
            lcd.print(words[songSequences[index][1]]);        
            lcd.setCursor(0, 1);
            lcd.print(words[songSequences[index][2]]);
    }
    

}
