#ifndef MENUVIEW_H
#define MENUVIEW_H
#include "gui.h"


// MENÜMODUS
class MenuView{
  public:
  Button* betriebButton;
  Button* kalibrierungButton;
  Button* protokollausgabeButton;

  MenuView(){
    this->betriebButton = new Button(25, 30, 260,  40, "   BETRIEB");
    this->kalibrierungButton = new Button(25, 90, 260,  40, " KALIBRIERUNG");
    this->protokollausgabeButton = new Button(25, 150, 260, 40, "   PROTOKOLL");
  }

  void display(){
    tft.fillScreen(BLACK);
    betriebButton->draw();
    kalibrierungButton->draw();
    protokollausgabeButton->draw();
    delay(20);
  }

  void loop(double volume, double water, double air1, double air2, TSPoint *p){
    if(p != NULL && p->z>0){ // screen is touched
      if(betriebButton->isPressedBy(p->x, p->y)){
        viewChanged = true;
        currentView = STANDBYVIEW;
        standbyView->display();
      }
      else if(kalibrierungButton->isPressedBy(p->x, p->y)){
        viewChanged = true;
        currentView = KALIBRIERUNGSVIEW;
        protokollView->display(); 
      }
      else if(protokollausgabeButton->isPressedBy(p->x, p->y)){
        viewChanged = true;
        currentView = PROTOKOLLVIEW;
        protokollView->display();  
      }
    }

  }
};
MenuView* menuView;


#endif