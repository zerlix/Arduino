#include "logger.h"
#include "shell.h"
#include <SPI.h>
#include <SdFat.h>


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


SdFat sd;  // SD Karte  
Shell  s;  // CLI / Shell 
Logger l;  // logger



void setup() {

  // Initialisierung der seriellen Konsole
  Serial.begin(115200);
  while (!Serial){;}

  // Initialisierung SD Karte
  if (!sd.begin(SD_CS)) {
    Serial.println(F("Kann SD Karte nicht ansprechen"));
  }
  l.writeLog("test");

}



void loop() {

    // Falls verfügbar commando aus der Seriellen 
    // Konsole lesen und ausführen
    s.getCommand();

}
