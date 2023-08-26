#include "logger.h"

Logger::Logger() {}

/**
  * @todo 
  * Datei darf nicht grösser als 100mb werden. Falls der Wert erreicht wurde muss 
  * eine neue Datei erstellt werden. Am besten durchnummerieren. 
  * Vielleicht auf der SD Karte einen config file erstellen. Wo der aktuelle "dateizählerstand"
  * gespeichert wird, um schneller den aktuellen log file zu finden.
  *
  * char _dataFileName[13] = "sdata_1.csv";  // logger.h
  * 
  * bool Logger::checkFileSize()
  * bool Logger::createnewLogFile()
  */
bool Logger::init() {
  
  _readCfgFile();
  // @todo:_dataFileName muss auf die aktuelle log datei zeigen
  if(_openlogFile() == false) {
    return false;
  }
  return true;

}

bool Logger::_readCfgFile() {
  _cfgFile = SD.open(_cfgFileName, FILE_WRITE);
  _cfgFile
}

// öffnet die logdatei
bool Logger::_openlogFile(){
  if (SD.begin(CS_PIN)) {
    _dataFile = SD.open(_dataFileName, FILE_WRITE);
    _dataFile.close();
    return true;
  }
  return false;
}

  


// schreibt zeile in datei
bool Logger::writeLog(String sensorValue) {
  
  _dataFile = SD.open(_dataFileName, FILE_WRITE);
  if (_dataFile) {
    _dataFile.println(sensorValue);
    _dataFile.close();
    return true;
  } else {
    return false;
  }
}


//dateigrösse ermitteln
bool Logger::checkFileSize() {
  if (_dataFile) {
    _dataFile.close();
  }
  _dataFile = SD.open(_dataFileName, FILE_READ);
  if (_dataFile) {
    int fileSize = _dataFile.size();
    _dataFile.close();
    if (fileSize >= 100000000) {  // 100 MB
      return false;  // Dateigröße erreicht.
  }
  return true;
  }
}