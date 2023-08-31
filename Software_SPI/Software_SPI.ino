#include <SPI.h>             
#include <SdFat.h>           

// SPI Pins 
const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN = 13;

// Chip select 
const uint8_t SD_CS_PIN = 10; // SD Karte

// Check SdFatConfig.h, ob Software SPI aktiviert wurde 
#if SPI_DRIVER_SELECT != 2
#error  SPI_DRIVER_SELECT muss auf 2 stehen- SdFatConfig.h 
#endif

// Chipselect Pins
#define SD_CS SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi) // Sd Karten Slot

SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi; //Pin belegung vom SD Karten Slot 

SdFat SD;
FsFile file;

void setup() {
  
    Serial.begin(9600);



    if (!SD.begin(SD_CS)) {
      SD.initErrorHalt();
    }

    if (!file.open("SoftSPI.txt", O_RDWR | O_CREAT)) {
      SD.errorHalt(F("open failed"));
    }

    while (file.available()) {
      Serial.write(file.read());
    }

    file.close();
}

void loop() {
  // put your main code here, to run repeatedly:

}
