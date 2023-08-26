#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include <SD.h>
#include "config.h"


class Logger {
public:
  
  Logger();
  bool init();
  bool writeLog(String sensorValue);
  bool checkFileSize();
  
private:
  File _dataFile, _cfgFile;
  char _dataFileName[13] = "sdata_1.csv";
  bool _openlogFile();

};

#endif

