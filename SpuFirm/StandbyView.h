#ifndef STANDBYVIEW_H
#define STANDBYVIEW_H

// STANDBYMODUS
class StandbyView{
  private:
    double prevVolume = 0;
    double prevWaterPressure = 0;
    double prevAir1 = 0;
    double prevAir2 = 0;
    double prevTemperature = 0;
    double prevAmbientPressure = 0;
    Button* menuButton;

  public:
    StandbyView(){
        menuButton = new Button(65, 200, 185, 40, "   MENU");
    }

    void display(){
      //BILDSCHIRM SCHWARZ EINFÄRBEN
      tft.fillScreen(BLACK);

      //TEXTAUSGABE
      tft.setCursor(10,STANDBYVIEW_YOFFSET);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("VOL = ");

      tft.setCursor(10,STANDBYVIEW_YOFFSET + 30*1);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("PWA = ");

      tft.setCursor(10,STANDBYVIEW_YOFFSET + 30*2);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("PL1 = ");

      tft.setCursor(10,STANDBYVIEW_YOFFSET + 30*3);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("PL2 = ");


      tft.setCursor(10,STANDBYVIEW_YOFFSET + 30*4);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("TMP = ");

      tft.setCursor(10,STANDBYVIEW_YOFFSET + 30*5);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("PUM = ");

      menuButton->draw();
        
      delay(20);
    }
    void loop(double volume, double waterPressure, double air1, double air2, double ambientPressure, double ambientTemperature, TSPoint *pressPointTft){
      
      //TEXTAUSGABE VOLUMENSTROM
      printValue(String(volume).c_str(), 0, volume!=this->prevVolume, "l/min");
      this->prevVolume = volume;

      //TEXTAUSGABE DRUCK WASSER
      if(abs(waterPressure-this->prevWaterPressure)>0.03*this->prevWaterPressure) {
          this->prevWaterPressure = 0.6*this->prevWaterPressure + 0.4*waterPressure;   // Tiefpassfilter
          printValue(String(this->prevWaterPressure).c_str(), 1, true, "mbar");
      }
      //TEXTAUSGABE DRUCK LUFT 1
      if(abs(air1-this->prevAir1)>0.03*this->prevAir1 || viewChanged){
          this->prevAir1 = 0.6*this->prevAir1 + 0.4*air1;   // Tiefpassfilter
          printValue(String(this->prevAir1).c_str(), 2, true, "mbar");  
      }
      //TEXTAUSGABE DRUCK LUFT 2
      if(abs(air2-this->prevAir2)>0.03*this->prevAir2 || viewChanged){
          this->prevAir2 = 0.6*this->prevAir2 + 0.4*air2;   // Tiefpassfilter
          printValue(String(this->prevAir2).c_str(), 3, true, "mbar");  
      }
      //TEXTAUSGABE TEMPERATUR
      if(abs(ambientTemperature-this->prevTemperature)>0.03*this->prevTemperature || viewChanged){
          this->prevTemperature = 0.6*this->prevTemperature + 0.4*ambientTemperature;   // Tiefpassfilter
          printValue(String(this->prevTemperature).c_str(), 4, true, "\xF7""C");
      }
      //TEXTAUSGABE UMGEBUNGSDRUCK      
      if(abs(ambientPressure-this->prevAmbientPressure)>0.03*this->prevAmbientPressure || viewChanged){
          this->prevAmbientPressure = 0.6*this->prevAmbientPressure + 0.4*ambientPressure;   // Tiefpassfilter
          printValue(String(prevAmbientPressure).c_str(), 5, true, "mbar");
      }

      viewChanged = false;

       if(pressPointTft != NULL && pressPointTft->z>0){ // screen is touched
        if(menuButton->isPressedBy(pressPointTft->x, pressPointTft->y)){
          viewChanged = true;
          currentView = MENUVIEW;
          //menuView->display();
        }
      }
    }
};
StandbyView* standbyView;

#endif