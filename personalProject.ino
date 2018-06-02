//www.elegoo.com
//2016.12.9

#include <SimpleDHT.h>
#include "pitches.h"

// for DHT11, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 2;
SimpleDHT11 dht11;

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int clock = 0;
int twentyMins = 20 * 60;
int greenLed = 13;
int redLed = 12;

// --------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  tone(8, NOTE_C4, 200);
  delay(150);
  tone(8, NOTE_G3, 200);
  delay(2000);
}

// --------------------------------------------------------------------------
void loop() {
  Serial.print(clock);
  clocker();
  readTempAndHumidity();
  delay(1100);
}

// --------------------------------------------------------------------------
void clocker() {
  clock++;
  if(clock == twentyMins) {
    sevenTones();
    clock = 0;
  }
}

// --------------------------------------------------------------------------
void readTempAndHumidity() {
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    Serial.print("Read DHT11 failed");
    turnOnLed(redLed,500);
    turnOffLed(redLed);
    delay(1500);
    return;
  }
  
  Serial.print("  ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" %");
  turnOnLed(greenLed,500);
  turnOffLed(greenLed);
}

// --------------------------------------------------------------------------
void sevenTones() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(8);
  }
}

// --------------------------------------------------------------------------

void turnOnLed(int ledPosition, int d) {
  digitalWrite(ledPosition, HIGH);
  delay(d);
}

// --------------------------------------------------------------------------

void turnOffLed(int ledPosition) {
  digitalWrite(ledPosition, LOW);
}
