//#include <stdio.h>
//#include <Arduino.h>

/** CLI **/
#include <cmdArduino.h>

/** Display **/
#include <TouchScreen.h>
#include <SPI.h>
#include <SD.h>

/** Eigene Header **/
#include "config.h"
#include "logger.h"
#include "commands.h"
#include "sensoren.h"


// These are the pins for the Touchscreen
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin
#define MINPRESSURE 200
#define MAXPRESSURE 2000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
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

  // Display Initalisieren
  //tft.begin();
}





void loop() {

  // Touchscreen
   TSPoint p = ts.getPoint();
  
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
     Serial.print("X = "); Serial.print(p.x);
     Serial.print("\tY = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);
  }
  

  // Serielle Eingabe holen und gegebenfalls Kommando ausführen.
  cmd.poll();

  // csv formatierte sensordaten holen und in datei schreiben
  l.writeLog(read_CSV_Sensor_Data());

  delay(500);
}
