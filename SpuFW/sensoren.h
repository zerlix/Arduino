#ifndef sensoren_h
#define sensoren_h

#include "config.h"


/**
  String read_CSV_Sensor_Data(void)
  @return String - enthält eine Zeile von Sensordaten
  z.b: 648,704,550
**/
String read_CSV_Sensor_Data(void) {
  
  
  /** @todo Sensordaten auslesen **/
  String w =  (String)random(1000) ; // Water
  String a =  (String)random(1000) ; // Air
  String f =  (String)random(1000) ; // Foam
  
  //erzeuge CSV Zeile 
  String s = w + "," + a + "," + f;
  
  return (s);
}

#endif

