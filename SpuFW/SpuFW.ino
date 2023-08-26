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

  // Initialisiere CLI (Command Line Interface)
  cmd.begin(9600);

  // auf console warten
  while (!Serial);

  // Stelle CLI kommandos bereit.
  cmd.add("ls", ls);
  cmd.add("free", free);

   // Logfile initialisieren 
  if ( l.init() == false ) {
    Serial.println("ERROR: Kann SD Karte nicht finden.");
  } else {
    Serial.println("SD Karte bereit...");
  }

}





void loop() {

  
  // Serielle Eingabe holen und gegebenfalls Kommando ausführen.
  cmd.poll();

  // csv formatierte sensordaten holen und in datei schreiben
  String csv = read_CSV_Sensor_Data();
  //Serial.println(csv);
  if (l.writeLog(csv) ) {
    //Serial.println("ERROR: writing log...");
  } else {
    Serial.println("ERROR: writing log...");
  }
  delay(500);

}
