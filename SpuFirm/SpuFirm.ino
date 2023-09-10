#include "shell.h"
#include "logger.h"
#include "gui.h"
#include "sensors.h"
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_BMP085.h>


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
  
  tft.begin(tft.readID()); 
  tft.setRotation(3);


  // init Views
  menuView = new MenuView();
  standbyView = new StandbyView(); 
}



void loop() {

  // Falls verfügbar commando aus der Seriellen 
  // Konsole lesen und ausführen
  s.getCommand();

  TSPoint pressPointTft = ts.getPoint();  //Get touch point
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  digitalWrite(YP, HIGH);  //because TFT control pins
  digitalWrite(XM, HIGH);
  
  ////ÜBERPRÜFEN OB DRUCKKRAFT GRÖSSER ALS 0 WAR UND FALLS JA:
  if (pressPointTft.z > 200) {

    Serial.print(pressPointTft.x);
    Serial.print("/");
    Serial.println(pressPointTft.y);

    //UMRECHNEN DER DRUCKPUNKTKOORDINATEN AUF DAS DISPLAYFORMAT
    
    //pressPointTft.x = map(pressPointTft.x, TS_MAXX, TS_MINX, 0, 320);
    //pressPointTft.y = map(pressPointTft.y, TS_MAXY, TS_MINY, 0, 240);
    //pressPointTft.y = 240-pressPointTft.y; // flip y axis orientation

    pressPointTft.x  = map(pressPointTft.x, 974, 218, 0, 320);
    pressPointTft.y  = map(pressPointTft.y, 904, 179, 0, 240);
    pressPointTft.y = 240-pressPointTft.y; // flip y axis orientation


    

  }
  //ABFRAGE SENSOREN
  double waterVolume= getWaterVolume();
  
  SensorValueWa = analogRead(SensorPinWa);    //Abfrage Drucksensor Wasser
  SensorValueL1 = analogRead(SensorPinL1);    //Abfrage Drucksensor Luft 1
  SensorValueL2 = analogRead(SensorPinL2);    //Abfrage Drucksensor Luft 2
  
  // Auswertung
  double waterPressure = scaleOffset(SensorValueWa, ScaleWa, OffsetWa);
  double air1Pressure = scaleOffset(SensorValueL1, ScaleL1/1000, 0);
  double air2Pressure = scaleOffset(SensorValueL2, ScaleL2/1000, 0);
  
  
  /** Blockiert die Ausführung wenn Sensor nicht angesprochen werden kann
  double ambientPressure = bmp.readPressure()/100; 
  double ambientTemperature = bmp.readTemperature();
  **/
  double ambientPressure = 100; 
  double ambientTemperature = 30;
  
  
  switch(currentView){
    case STANDBYVIEW:
      if(viewChanged) {
        standbyView->display();
      };
      standbyView->loop(waterVolume, waterPressure, air1Pressure, air2Pressure, ambientPressure, ambientTemperature, &pressPointTft);
      break;
    case MENUVIEW:
      if(viewChanged) {
       menuView->display();
       viewChanged = false;
      };
      menuView->loop(waterVolume, waterPressure, air1Pressure, air2Pressure, &pressPointTft);
      break;
  }
  delay(50);
}
