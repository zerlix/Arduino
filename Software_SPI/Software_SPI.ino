#include <SPI.h>             
#include <SdFat.h>

// Software SPI muss in der Datei ..\Arduino\libraries\SdFat\src\SdFatConfig.h aktiviert werden
// dazu muss SPI_DRIVER_SELECT auf 2 gesetzt werden.
#if SPI_DRIVER_SELECT != 2
#error  SPI_DRIVER_SELECT != 2
#endif

// SPI Pins 
const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN = 13;

// Chip select 
const uint8_t SD_CS_PIN = 10; // SD Karte

// Chipselect Pins
#define SD_CS SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi) // Sd Karten Slot

SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi; //Pin belegung vom SD Karten Slot 

SdFat sd;
FsFile file;

void setup() {
  
    Serial.begin(9600);


    if (!sd.begin(SD_CS)) {
      sd.initErrorHalt();
    }

    // Inhaltsverzeicvhnis der SD Karte auf Serielle Konsole ausgeben
    sd.ls(LS_SIZE|LS_R|LS_DATE);


    // Datei öffnen
    if (!file.open("SoftSPI.txt", O_RDWR | O_CREAT)) {
      sd.errorHalt(F("open failed"));
    }


    // in Datei schreiben.
    file.println("1,2,3 Test");
    
    // Position am anfang der datei setzen
    file.rewind();

    // Datei ausgeben
    while (file.available()) {
      Serial.write(file.read());
    }

    // Datei schliessen
    file.close();

    //sd.remove("SoftSPI.txt");

}

void loop() {
  // put your main code here, to run repeatedly:

}

