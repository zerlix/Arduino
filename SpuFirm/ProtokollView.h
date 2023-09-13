#ifndef PROTOKOLLVIEW_H
#define PROTOKOLLVIEW_H
#include "gui.h"


// MENÜMODUS
class ProtokollView 
{
  private:
    Button* menuButton;

  public:
    ProtokollView(){
        menuButton = new Button(65, 200, 185, 40, "   MENU");
    }

    void display() 
    {
      tft.fillScreen(BLACK);
      tft.setCursor(10,STANDBYVIEW_YOFFSET);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("Protokoll");
      menuButton->draw();
    }

    
    void loop(TSPoint* pressPointTft) 
    {

      if (pressPointTft != NULL && pressPointTft->z > 0) {  
        if (menuButton->isPressedBy(pressPointTft->x, pressPointTft->y)) {
          viewChanged = true;
          currentView = MENUVIEW;
        }
      }
    }
};

ProtokollView* protokollView;

#endif