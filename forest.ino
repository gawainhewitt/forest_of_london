#include "constants.h"
#include "mpr121.h"
#include "wavFilePlayer.h"
#include "reboot.h"

#define Wire Wire1 

void setup() {
  Wire.begin();        
  Serial.begin(9600);  
  init_mpr121();
  init_player();
  pinMode(rebootButton, INPUT_PULLUP);
  pinMode(volumePin, INPUT);
  delay(500);
}

void changeVolume() {
  int knob = analogRead(volumePin); // knob = 0 to 1023
  float gain = (float)knob / 1023.0;
  amp1.gain(gain);
  amp2.gain(gain);
}

void touchFunction() {
  currtouched1 = mprBoard_A.touched();

  for (uint8_t i=0; i<12; i++) {
    if ((currtouched1 & _BV(i)) && !(lasttouched1 & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched of A");
      if(lastCard != 0){
        playSound(i);
      }
    }

    if (!(currtouched1 & _BV(i)) && (lasttouched1 & _BV(i)) ) {
      Serial.print(i); Serial.println(" released of A");
      stopSound(i);
    }
  }

  lasttouched1 = currtouched1;
}

void loop() {
  if(digitalRead(rebootButton) == LOW){
    doReboot();
  }
  changeVolume();
  touchFunction();
}
