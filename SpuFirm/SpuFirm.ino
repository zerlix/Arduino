#include "logger.h"
#include "shell.h"

/**
*** COPY-PASTE from Serial Terminal:
const int TS_LANDSCAPE=1; //TouchScreen.h GFX Calibration
const int TS_LEFT=936,TS_RT=182,TS_TOP=171,TS_BOT=941;

PORTRAIT  CALIBRATION     240 x 320
x = map(p.x, LEFT=936, RT=182, 0, 240)
y = map(p.y, TOP=171, BOT=941, 0, 320)

LANDSCAPE CALIBRATION     320 x 240
x = map(p.y, LEFT=171, RT=941, 0, 320)
y = map(p.x, TOP=182, BOT=936, 0, 240)


*** COPY-PASTE from Serial Terminal:
const int TS_LANDSCAPE=1; //TouchScreen.h GFX Calibration
const int TS_LEFT=174,TS_RT=932,TS_TOP=928,TS_BOT=170;

PORTRAIT  CALIBRATION     240 x 320
x = map(p.x, LEFT=174, RT=932, 0, 240)
y = map(p.y, TOP=928, BOT=170, 0, 320)

LANDSCAPE CALIBRATION     320 x 240
x = map(p.y, LEFT=928, RT=170, 0, 320)
y = map(p.x, TOP=932, BOT=174, 0, 240)

**/

Logger l;
Shell  s;


void setup() {

  Serial.begin(115200);

  while (!Serial) {;}

}

void loop() {

    s.getCommand();
   
    //Serial.println("1");
}
