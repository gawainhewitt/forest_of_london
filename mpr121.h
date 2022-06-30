#include <bus1_MPR121.h> 
#include <Wire.h>

Adafruit_MPR121 mprBoard_A = Adafruit_MPR121();

uint16_t lasttouched1 = 0;
uint16_t currtouched1 = 0;

void init_mpr121()
{
    if (!mprBoard_A.begin(0x5A)) {
    Serial.println("MPR121 A not found, check wiring?");
    while (1);
    }
    Serial.println("MPR121 A found!");

    mprBoard_A.writeRegister(MPR121_ECR, 0x00); 
    mprBoard_A.setThreshholds(0x05, 0x00); 
    mprBoard_A.writeRegister(MPR121_ECR, 0x8F); 

}
