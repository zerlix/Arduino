#ifndef GUI_H
#define GUI_H
#include <Arduino.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>

//FARBDEFINITIONEN
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

const int XP=6,XM=A2,YP=A1,YM=7;
const int TS_LEFT=181,TS_RT=914,TS_TOP=978,TS_BOT=203;

extern MCUFRIEND_kbv tft;

enum views{
  MENUVIEW,
  STANDBYVIEW
};

void printText(int xOffset, int yOffset, char* text, int textSize=3);
void printValue(const char* value, int rowIndex, bool marked, const char* unit);

#include "views/button.h"


// ZEILENABSTAND FÜR DIE MESSWERTANZEIGE IM STANDBYMODUS FESTLEGEN
int STANDBYVIEW_YOFFSET = 20;


// VORBELEGUNG VON WERTEN FÜR DIE BILDSCHIRMANSICHT
bool viewChanged = true;
int currentView = STANDBYVIEW;



// TEXTAUSGABE
void printText(int xOffset, int yOffset, char* text, int textSize=3){
    tft.setTextColor(WHITE);
    tft.setTextSize(textSize);
    tft.setCursor(xOffset, yOffset);
    tft.print(text);
};


// WERTEAUSGABE
void printValue(const char* value, int rowIndex, bool marked, const char* unit){

      int xOffset = 120;
      int yOffset = STANDBYVIEW_YOFFSET+rowIndex*30;
      int xOffsetUnit = 260;
      int yOffsetUnit = STANDBYVIEW_YOFFSET + 6+rowIndex*30;
      
      if(marked) tft.fillRect(xOffset, yOffset, 130, 21, RED);   

      printText(xOffset, yOffset, value);
      
      if(unit=="") return;

      printText(xOffsetUnit, yOffsetUnit, unit, 2);
}

#endif