#ifndef KALIBRIERUNGSVIEW_H
#define KALIBRIERUNGSVIEW_H
#include "gui.h"


// MENÜMODUS
class KalibrierungsView 
{
  private:
    Button* menuButton;

  public:
    KalibrierungsView(){
        menuButton = new Button(65, 200, 185, 40, "   MENU");
    }

    void display() 
    {
      tft.fillScreen(BLACK);
      tft.setCursor(10,STANDBYVIEW_YOFFSET);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("Kalibrierung");
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

KalibrierungsView* kalibrierungsView;

#endif