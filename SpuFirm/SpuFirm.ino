#include <Arduino.h>
#include <Streaming.h>
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_BMP085.h>
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

// SPI Pins (SD Card)
const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN  = 13;
// Chip select Pin (SD Card)
const uint8_t SD_CS_PIN     = 10;  


// Chipselect Pin Macro für SD Karten Slot definieren
#define SD_CS SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi)

// C++ Template, erstellt eine Klasse SoftSpiDriver
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi; //Pin belegung  SD Karten Slot 

MCUFRIEND_kbv tft; // Display
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // Touchscreen
TSPoint pressPointTft;

SdFat sd;  // SD Karte  
Shell  s;  // CLI / Shell 
Logger l;  // logger
Adafruit_BMP085 bmp; //bmp Sensor


void setup() {

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
  s.getCommand();

  // Lese x/y Koordinaten, falls auf das touchpad geklickt wurde
  getXY();
 
  // Schreibe Sensordaten ins Logfile
  if(! l.writeLog(getCsvSensorData())) {
    Serial.println("Error:");
  }
  
   
  // liest die aktuellen Sensordaten 
  // noch nötig ???? 
  // Sonsordaten werden schon beim aufruf von getCsvSensorData() aktualisiert
  // getSensorData();


  // Welcher View soll angezeigt werden  
  switch(currentView){
    case STANDBYVIEW:
      if(viewChanged) {
        standbyView->display();
      }
      standbyView->loop(waterVolume, waterPressure, air1Pressure, air2Pressure, ambientPressure, ambientTemperature, &pressPointTft);
      break;
    case MENUVIEW:
      if(viewChanged) {
       menuView->display();
       viewChanged = false;
      }
      menuView->loop(waterVolume, waterPressure, air1Pressure, air2Pressure, &pressPointTft);
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
