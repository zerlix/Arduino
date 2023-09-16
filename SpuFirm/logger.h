#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>

extern SdFat sd; 

/**
 * @class Logger
 * einfache Logfile Klasse.
 * TODO: 
 *      - Neuer Logfile bei Max Dateigrösse
 *      - einfache "tail/cat" funtion
 **/

class Logger {

public:
  Logger();                        // Konstruktur
  bool writeLog(const char* str);  // schreibt in den Logfile

private:
  FsFile logFile;                  // Datei Handle
  char logFileName[13] = "1.csv";  // Logfile Name
};

#endif
