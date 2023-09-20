#include <Arduino.h>
#include <Streaming.h>
#include <SPI.h>
#include <SdFat.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include "shell.h"
#include "logger.h"
#include "gui.h"
#include "sensors.h"


// Software SPI muss in der Datei ..\Arduino\libraries\SdFat\src\SdFatConfig.h aktiviert werden
// dazu muss SPI_DRIVER_SELECT auf 2 gesetzt werden.
#if SPI_DRIVER_SELECT != 2
#error SPI_DRIVER_SELECT != 2
#endif

// C++ Template, erstellt eine Klasse SoftSpiDriver
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi; //Pin belegung  SD Karten Slot 


MCUFRIEND_kbv tft; // Display
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // Touchscreen
TSPoint pressPointTft; //
SdFat sd;  // SD Karte  
Shell  cli;  // CLI / Shell 
Logger l;  // logger
Sensors s;


void setup() 
{

  pinMode(33, INPUT);

  // Initialisierung der seriellen Konsole
  Serial.begin(115200);
  while (!Serial){
    Serial.println("Error");
  }

  // Initialisierung SD Karte
  if (!sd.begin(SD_CS)) {
    Serial.println(F("Kann SD Karte nicht ansprechen"));
  }
  
  // init Display
  tft.begin(tft.readID()); 
  tft.setRotation(3);


  // init Views
  menuView = new MenuView();
  standbyView = new StandbyView(); 
  protokollView = new ProtokollView();
  kalibrierungsView = new KalibrierungsView();

}



void loop() {

  // Falls verfügbar commando aus der Seriellen 
  // Konsole lesen und ausführen
  cli.getCommand();

  
  // Lese x/y Koordinaten, falls auf das touchpad geklickt wurde
  getXY();
 

  // TODO: Sensor klasse überarbeiten
  s.getSensorData();

  // Schreibe Sensordaten ins Logfile
  if(! l.writeLog(s.getCsvSensorData()) ) {
    Serial.println("Error:");
  }
  
  // Welcher View soll angezeigt werden  
  switch(currentView){
    case STANDBYVIEW:
      if(viewChanged) {
        standbyView->display();
      }
      standbyView->loop(s.waterVolume, s.waterPressure, s.air1Pressure, s.air2Pressure, s.ambientPressure, s.ambientTemperature, &pressPointTft);
      break;
    case MENUVIEW:
      if(viewChanged) {
       menuView->display();
       viewChanged = false;
      }
      menuView->loop(s.waterVolume, s.waterPressure, s.air1Pressure, s.air2Pressure, &pressPointTft);
      break;
    case PROTOKOLLVIEW:
      if(viewChanged) {
       protokollView->display();
       viewChanged = false;
      }
      protokollView->loop(&pressPointTft);
      break;
    case KALIBRIERUNGSVIEW:
      if(viewChanged) {
       kalibrierungsView->display();
       viewChanged = false;
      }
      kalibrierungsView->loop(&pressPointTft);
      break;
  }
  delay(50);
}
