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
  */
bool Logger::init() {
  
  // SD Slot initialisieren
  if (SD.begin(CS_PIN)) {
    Serial.println("SD Slot ready...");
  }
  

  // Versuchen cfgFileName zu öffnen
  _cfgFile = SD.open(_cfgFileName, FILE_READ);
  
  // Falls es kein cfgFile auf der SD Card gibt...
  if(!_cfgFile.available() ) {
    // ... dann erstellen wir einen
    Serial.println("Erstelle konfigurationsdatei");
    _createCfgFile(); 
    _cfgFile.close();
  } else {
    Serial.println("Lese konfiguration ein.");
  }
    

  // cfgFile löschen nur für debugg
  //SD.remove("logger.cfg");


  // @todo:_dataFileName muss auf die aktuelle log datei zeigen
  if(_openlogFile() == false) {
    return false;
  }
  return true;

}

// öffnet die logdatei
bool Logger::_openlogFile(){
   
  _dataFile = SD.open(_dataFileName, FILE_WRITE);
  if(_dataFile) {
    _dataFile.close();
    return true;
  } else {
    return false;
  }
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


/**
 * wird nur aufgerufen wenn kein cfg file existiert.
 */
bool Logger::_createCfgFile() {
 
  // cfg file öffnen
  _cfgFile = SD.open(_cfgFileName, FILE_WRITE);
  
  // Hat das öffnen der Datei geklappt, schreiben wir 
  // in die datei (_dataFileName:sdata_1.csv), 
  // schliessen die datei und geben true zurück
  if(_cfgFile) {
    _cfgFile.println("_dataFileName:sdata_1.csv");
    _cfgFile.close();
    return true;
  } else {
    // Fehler ist aufgetreten
    Serial.println("ERROR: Config File kann nicht erstellt werden !!");
    Serial.flush();
    return false; 
  }
 
  return false;  // Fehler ist aufgetreten
}