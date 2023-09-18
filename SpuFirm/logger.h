#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>


// SPI Pins (SD Card)
const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN  = 13;
// Chip select Pin (SD Card)
const uint8_t SD_CS_PIN     = 10;  

// Chipselect Pin Macro für SD Karten Slot definieren
#define SD_CS SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi)



extern SdFat sd; 

/**
 * @class Logger
 * einfache Logfile Klasse.
 * TODO: 
 *      - Neuer Logfile bei Max Dateigrösse
 *      - einfache "tail/cat" funtion
 **/

class Logger {

private:
  FsFile logFile;                  // Datei Handle
  char logFileName[13] = "1.csv";  // Logfile Name

public:

/**
 * Konstruktor Methode wird automatisch aufgerufen
 * wenn ein Object von der Klasse "Logger" erstellt wird
 **/
Logger::Logger() 
{

}



/** 
 * Schreibt eine Zeile in die logdatei
 * \param[in] str zeile die in die datei geschrieben werden soll
 * \return true|false
 **/
bool Logger::writeLog(const char* str) {
  // Öffne des Logfiles
  if (!logFile.open(logFileName, O_RDWR | O_CREAT | O_APPEND)) {
    Serial.println(F("Logger:writeLog() - Kann Datei nicht öffnen"));
    return false;  // funktion beenden und false zurückgeben
  }

  // Schreibe die daten in die datei
  logFile.write(str);
  logFile.write('\n'); // Zeilenumbruch (newline)
  
  // Sicherstellen das der Buffer geleert und die datei vollständig geschrieben wurde
  logFile.sync();

  // Falls Fehler aufgetreten sind. Meldung auf der Konsole ausgeben
  if (logFile.getWriteError()) {
    Serial.println(F("Logger::write() - Kann die Datei nicht schreiben"));
    return false;  // funktion beenden und false zurückgeben
  }

  // Datei schliessen um Speicherplatz freizugeben
  logFile.close();

  // Wenn wir hier angekommen sind hat alles geklappt und geben ein true zurück
  return true;
}
};

#endif
