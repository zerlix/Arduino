#ifndef config_h
#define config_h


#define CS_PIN 10

/** Display unnd Tochscreen config
*** COPY-PASTE from Serial Terminal:
const int XP=6,XM=A2,YP=A1,YM=7; //240x320 ID=0x9341
const int TS_LEFT=181,TS_RT=914,TS_TOP=978,TS_BOT=203;

PORTRAIT  CALIBRATION     240 x 320
x = map(p.x, LEFT=181, RT=914, 0, 240)
y = map(p.y, TOP=978, BOT=203, 0, 320)

LANDSCAPE CALIBRATION     320 x 240
x = map(p.y, LEFT=978, RT=203, 0, 320)
y = map(p.x, TOP=914, BOT=181, 0, 240)
**/

#endif
