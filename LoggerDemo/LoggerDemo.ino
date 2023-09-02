#include "logger.h"


// erstellen ein Objekt der Logger Klasse
Logger L;  


void setup() {


  // Initialisieren der seriellen console
  Serial.begin(9600);
  while (!Serial);

  // eine Zeile in die Logdatei zu schreiben
  L.writeLog("test");


  /****************************** 
   * Gibt den Logfile auf der Console aus.
   * Wird nur für Testzwecke genutzt. 
   * Kann, bis zu BLOCK ENDE, gelöscht werden.
   **/
  FsFile file; // Filehandle
  SdFat sd;  // SD Card
  // Datei öffnen
  if (!file.open("1.csv", O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }
 
 // Datei ausgeben
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
  /*********** BLOCK ENDE **********/


}


void loop() {
}


