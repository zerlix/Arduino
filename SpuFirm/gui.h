#ifndef GUI_H
#define GUI_H
#include <Arduino.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include <Streaming.h>

//FARBDEFINITIONEN
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define TS_MINX 130
#define TS_MINY 110
#define TS_MAXX 950
#define TS_MAXY 930
#define MINPRESSURE 200
#define MAXPRESSURE 1000

//PINBELEGUNG FÜR DAS DISPLAY FESTLEGEN
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

//PINBELEGUNG FÜR DEN TOUCHSCREEN FESTLEGEN
#define YP A2
#define XM A3
#define YM 8
#define XP 9

//X- UND Y-GRENZEN FÜR DEN TOUCHSCREEN FESTLEGEN
#define TS_MINX 130
#define TS_MINY 110
#define TS_MAXX 950
#define TS_MAXY 930

//const int XP=6,XM=A2,YP=A1,YM=7;
//const int TS_LEFT=181,TS_RT=914,TS_TOP=978,TS_BOT=203;

extern MCUFRIEND_kbv tft;
extern TouchScreen ts; // = TouchScreen(XP, YP, XM, YM, 300);
extern TSPoint pressPointTft;

// Views
enum views{
  MENUVIEW,
  STANDBYVIEW,
  PROTOKOLLVIEW,
  KALIBRIERUNGSVIEW
};

// ZEILENABSTAND FÜR DIE MESSWERTANZEIGE IM STANDBYMODUS FESTLEGEN
int STANDBYVIEW_YOFFSET = 20;

// VORBELEGUNG VON WERTEN FÜR DIE BILDSCHIRMANSICHT
bool viewChanged = true;
int currentView = STANDBYVIEW;


// deklaration von hilfsfunktionen
void printText(int xOffset, int yOffset, char* text, int textSize=3);
void printValue(const char* value, int rowIndex, bool marked, const char* unit);


// Views includieren
#include "Button.h"
#include "ProtokollView.h"
#include "KalibrierungsView.h"
#include "StandbyView.h"
#include "MenuView.h"





// TEXTAUSGABE
void printText(int xOffset, int yOffset, char* text, int textSize=3)
{
    tft.setTextColor(WHITE);
    tft.setTextSize(textSize);
    tft.setCursor(xOffset, yOffset);
    tft.print(text);
};


// WERTEAUSGABE
void printValue(const char* value, int rowIndex, bool marked, const char* unit)
{

      int xOffset = 120;
      int yOffset = STANDBYVIEW_YOFFSET+rowIndex*30;
      int xOffsetUnit = 260;
      int yOffsetUnit = STANDBYVIEW_YOFFSET + 6+rowIndex*30;
      
      if(marked) 
        tft.fillRect(xOffset, yOffset, 130, 21, RED);   

      printText(xOffset, yOffset, value);
      
      if(unit=="") 
        return;

      printText(xOffsetUnit, yOffsetUnit, unit, 2);
}

void getXY(void) {
 
  pressPointTft = ts.getPoint();
  
  pinMode(YP, OUTPUT);  //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);  //because TFT control pins
  digitalWrite(XM, HIGH);

    ////ÜBERPRÜFEN OB DRUCKKRAFT GRÖSSER ALS 0 WAR UND FALLS JA:
  if (pressPointTft.z > 200) {

    //UMRECHNEN DER DRUCKPUNKTKOORDINATEN AUF DAS DISPLAYFORMAT
    pressPointTft.x = map(pressPointTft.x, TS_MAXX, TS_MINX, 0, 320);
    pressPointTft.y = map(pressPointTft.y, TS_MAXY, TS_MINY, 0, 240);
    pressPointTft.y = 240-pressPointTft.y; // flip y axis orientation

  }

  /**
  pressPointTft.x= map(pressPointTft.y, TS_LEFT, TS_RT, 0, 320);
  pressPointTft.y = map(pressPointTft.x, TS_BOT, TS_TOP, 0, 240);
  **/
  //Serial << pressPointTft.x << "," << pressPointTft.y << "," << pressPointTft.z << endl;
  

}

#endif