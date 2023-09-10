#ifndef BUTTON_H
#define BUTTON_H
//#include "../gui.h"

//extern MCUFRIEND_kbv tft;
//extern void printText(int xOffset, int yOffset, char* text, int textSize=3); 
//extern void printValue(const char* value, int rowIndex, bool marked, const char* unit);

// ABFRAGEBUTTON
class Button{
  public:
    int x;
    int y; 
    int w;
    int h;
    const char* text;
    
    Button(int x, int y, int w, int h, const char* text){
      this->x = x;
      this->y = y;
      this->w = w;
      this->h = h;
      this->text = text;
    }
  
    void draw(){
      tft.fillRect(x, y, w, h, RED);
      tft.drawRoundRect(x, y, w, h, 4, WHITE);
      printText(x+5,y+10,text,3);
    }

    bool isPressedBy(int posX, int posY){
      return (posX-x >= 0 && posX-x <= w && posY-y >= 0 && posY-y <= h);
    }
};
#endif