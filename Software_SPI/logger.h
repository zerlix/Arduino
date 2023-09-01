#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>


// Software SPI muss in der Datei ..\Arduino\libraries\SdFat\src\SdFatConfig.h aktiviert werden
// dazu muss SPI_DRIVER_SELECT auf 2 gesetzt werden.
#if SPI_DRIVER_SELECT != 2
#error SPI_DRIVER_SELECT != 2
#endif

// SPI Pins
const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN = 13;
// Chip select Pin
const uint8_t SD_CS_PIN = 10;  // SD Karte


// Chipselect Pin Macro für SD Karten Slot definieren
#define SD_CS SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi)


/**
 * @class Logger
 * einfache Logfile Klasse.
 **/

class Logger {

public:
  Logger();                        // Konstruktur
  bool writeLog(const char* str);  // schreibt in den Logfile

private:
  SdFat sd;                        // SD Karte
  FsFile logFile;                  // Datei Handle
  char logFileName[13] = "1.csv";  // Logfile Name
};

#endif
