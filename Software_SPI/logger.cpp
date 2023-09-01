#include "logger.h"


/**
 * Konstruktor Methode wird automatisch aufgerufen
 * wenn ein Object von der Klasse "Logger" erstellt wird
 **/
Logger::Logger() {

  // Initialisierung SD Karte
  if (!sd.begin(SD_CS)) {
    Serial.println("Logger::Logger() - Kann SD Karte nicht ansprechen");
  }
}



/** 
 * Schreibt eine Zeile in die logdatei
 * \param[in] str zeile die in die datei geschrieben werden soll
 * \return true|false
 **/
bool Logger::writelog(const char* str) {
  // Öffne des Logfiles
  if (!logFile.open(logFile, O_RDWR | O_CREAT | O_APPEND)) {
    Serial.println(F("Logger:write() - Kann Datei nicht öffnen"));
    return false;  // funktion beenden und false zurückgeben
  }

  // Schreibe eine Zeile in die Log
  logFile.write(str);

  // Sicherstellen das der Buffer geleert und die datei vollständig geschrieben wurde
  logFile.sync();

  // Falls Fehler aufgetreten sind. Meldung auf der Console ausgeben
  if (logFile.getWriteError()) {
    Serial.println(F("Logger::write() - Kann die Datei nicht schreiben"));
    return false;  // funktion beenden und false zurückgeben
  }

  // Datei schliessen um Speicherplatz freizugeben
  logFile.close();

  // Wenn wir hier angekommen sind hat alles geklappt und geben ein true zurück
  return true;
}