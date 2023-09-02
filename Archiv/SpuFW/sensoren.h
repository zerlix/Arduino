#ifndef sensoren_h
#define sensoren_h

#include "config.h"


/**
  * String read_CSV_Sensor_Data(void)
  * @return String - enthält eine Zeile von Sensordaten
  * z.b: 648,704,550
**/
String read_CSV_Sensor_Data(void) {
  
  
  /** @todo Sensordaten auslesen **/
  String w1 =  (String)random(1000) ; // Water 1
  String w2 =  (String)random(1000) ; // Water 2
  String a1 =  (String)random(1000) ; // Air 1
  String a2 =  (String)random(1000) ; // Air 2
  String f1 =  (String)random(1000) ; // Foam 1
  String t1 =  (String)random(1000) ; // Temp 1 
  
  //erzeuge CSV Zeile 
  String s = w1 + "," + w2 + "," + a1 + "," + a2 + "," + f1 + "," + t1;
  
  return (s);
}

#endif

