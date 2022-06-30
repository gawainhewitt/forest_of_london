#include <Audio.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

const char *FOREST[] = {"FOREST0.WAV", "FOREST1.WAV", "FOREST2.WAV",
                     "FOREST3.WAV", "FOREST4.WAV", "FOREST5.WAV", "FOREST6.WAV",
                     "FOREST7.WAV", "FOREST8.WAV", "FOREST9.WAV", "FOREST10.WAV",
                     "FOREST11.WAV"}; 

const char *HUMAN[] = {"HUMAN0.WAV", "HUMAN1.WAV", "HUMAN2.WAV",
                     "HUMAN3.WAV", "HUMAN4.WAV", "HUMAN5.WAV", "HUMAN6.WAV",
                     "HUMAN7.WAV", "HUMAN8.WAV", "HUMAN9.WAV", "HUMAN10.WAV",
                     "HUMAN11.WAV"}; 

char *toPlay[48];

AudioPlaySdWav           playSdWav4;     
AudioPlaySdWav           playSdWav3;     
AudioPlaySdWav           playSdWav2;     
AudioPlaySdWav           playSdWav1;     
AudioMixer4              mixer1;         
AudioMixer4              mixer2;         
AudioAmplifier           amp1;           
AudioAmplifier           amp2;           
AudioOutputI2S           i2s1;           
AudioConnection          patchCord1(playSdWav4, 0, mixer1, 3);
AudioConnection          patchCord2(playSdWav4, 1, mixer2, 3);
AudioConnection          patchCord3(playSdWav3, 0, mixer1, 2);
AudioConnection          patchCord4(playSdWav3, 1, mixer2, 2);
AudioConnection          patchCord5(playSdWav2, 0, mixer1, 1);
AudioConnection          patchCord6(playSdWav2, 1, mixer2, 1);
AudioConnection          patchCord7(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord8(playSdWav1, 1, mixer2, 0);
AudioConnection          patchCord9(mixer1, amp1);
AudioConnection          patchCord10(mixer2, amp2);
AudioConnection          patchCord11(amp1, 0, i2s1, 0);
AudioConnection          patchCord12(amp2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

void init_player() {

  AudioMemory(8);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

int active[MAX_POLY];
long when[MAX_POLY];
long faded_ms[MAX_POLY];

byte get_free_voice() {
  for(byte voice=0;voice<MAX_POLY;voice++) {
    if(active[voice] == -1) return voice;
  }

   long oldest_time = millis();
  byte oldest = 0;
  for(byte voice=0;voice<MAX_POLY;voice++) {
    if(when[voice] < oldest_time) {
      oldest = voice;
      oldest_time = when[voice];
    }
  }
  active[oldest] = -1;
  return oldest;
}

void playSound(int track) {
    int player = get_free_voice();
    Serial.print("notezzzz");
    Serial.println(player);
    

        switch(player)
    {
        case 0:
            playSdWav1.play(toPlay[track]);
            active[0] = track;
            when[0] = millis();
            break;
        case 1:
            playSdWav2.play(toPlay[track]);
            active[1] = track;
            when[1] = millis();
            break;
        case 2:
            playSdWav3.play(toPlay[track]);
            active[2] = track;
            when[2] = millis();
            break;
        case 3:
            playSdWav4.play(toPlay[track]);
            active[3] = track;
            when[3] = millis();
            break;
    }
    Serial.print("active ");
    Serial.println(active[0]);
}

void stopSound(int track) {
    for(int i = 0; i < MAX_POLY; i++){
        if(active[i] == track){
            switch(i)
    {
        case 0:
            playSdWav1.stop();
            active[0] = -1;
            break;
        case 1:
            playSdWav2.stop();
            active[1] = -1;
            break;
        case 2:
            playSdWav3.stop();
            active[2] = -1;
            break;
        case 3:
            playSdWav4.stop();
            active[3] = -1;
            break;
    }
        }
    }
}
