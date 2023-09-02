/** CLI **/
#include <cmdArduino.h>

/** SD Card/Display **/
#include <SPI.h>
#include <SD.h>

/** Eigene Header **/
#include "config.h"
#include "logger.h"
#include "commands.h"
#include "sensoren.h"


Logger l;

void setup() {

    // SD Slot initialisieren
  if (SD.begin(CS_PIN)) {
    Serial.println("SD Slot ready...");
  }

  // Initialisiere CLI (Command Line Interface)
  cmd.begin(9600);

  // auf console warten
  while (!Serial);

  // Stelle CLI kommandos bereit.
  cmd.add("ls", ls);
  cmd.add("free", free);
  cmd.add("delcfg", delcfg);

   // Logfile initialisieren 
  if ( l.init() == false ) {
    Serial.println("ERROR Logger::init()");
  }

}





void loop() {

  
  // Serielle Eingabe holen und gegebenfalls Kommando ausführen.
  cmd.poll();

  // csv formatierte sensordaten holen.. 
  String csv = read_CSV_Sensor_Data();
  // ..und in datei schreiben
  if (!l.writeLog(csv) ) {
    Serial.println("ERROR: writing log...");
  } 

  
  delay(500);

}
