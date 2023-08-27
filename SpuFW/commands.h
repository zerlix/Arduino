#ifndef commands_h
#define commands_h
#include "config.h"

// kommandos
void free (int argc, char **argv); 
void ls( int argc, char **argv );
void delcfg(int argc, char **argv);

// interne hilfsfunktionen
void _printDirectory(File dir, int numTabs);
bool _isNumber(char *s); /* @todo Kann eigentlich entfernt werden ? */

/**
  * void freeRam(int argc, char **argv)
  * Kommando "free" gibt den freiem dynamischem Speicher (Heap) auf der Seriellen Konsole aus
  * @param int  argc - enthält die anzahl der Übergabeparameter
  * @param char **argv  -  pointer auf ein Arrray mit übergabeparameter
  * Infos um den SRAM (Heap) zu optimieren:
  * https://learn.adafruit.com/memories-of-an-arduino/optimizing-sram
  **/
void free (int argc, char **argv) {
  extern int __heap_start, *__brkval;
  int v;
  Serial.print((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
  Serial.println(" Bytes free");
}

/**
  * void ls(int argc, char **argv)
  * Kommando "ls" listet das Inhaltsverzeichnis der SD Karte auf
  * @param int  argc - enthält die anzahl der Übergabeparameter
  * @param char **argv  -  pointer auf ein Arrray mit übergabeparameter
  * @todo übergabeparameter funktioniert noch nicht
  **/
void ls( int argc, char **argv ) {
  File root;
  root = SD.open("/");
  _printDirectory(root, 0);
  root.close();
}

void delcfg (int argc, char **argv) {
  if(SD.remove("logger.cfg")){
    Serial.println("datei wurde gelöscht");
  } else {
    Serial.println("kann datei nicht löschen");
  }
}

/** void _printDirectory(File dir, int numTabs)
  * @param File Dir -- Verzeichnis das gelistet werden soll
  * @param int numTabs -- Var um Tabs beim listing zu ermöglichen
  * Hilfsfunktion für ls(), um rekrusiven aufruf zu ermöglichen
  * @todo funktion muss noch mit unterverzeichnisse getestet werden
  **/
void _printDirectory(File dir, int numTabs) {

  // Dateisystem Listen
  while (true) {
    
    // Nächsten Eintrag öffnen
    File entry = dir.openNextFile();

    // Wenn Entry leer sind wir am Ende angekommen und beenden die schleife
    if (!entry) { break; } 

    // tabs ausgeben für die formatierung der Ausgabe  
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }

    // Datei bzw Verzeichnisname ausgeben 
    Serial.print(entry.name());
    // Falls "entry" ein Verzeichnis ist. Ein backslash" ausgeben
    if (entry.isDirectory()) {
      Serial.println("/");
      // rekrusiver functionsaufruf um unterverzeichnis(entry) auszugeben 
      _printDirectory(entry, numTabs + 1); 
    } else {
      // Ansonsten ist es eine Datei, grösse ermitteln und ausgeben
      Serial.print("\t\t"); // Tabs für Formatierung
      Serial.println(entry.size(), DEC); // Grösse der Aktuellen Datei ausgeben 
    }
    entry.close(); // Datei schliessen
  }
}


/**
  bool _isNumber()
  @param char *s - pointer auf einen string
  @return true/false
  gibt true zurück wenn "s" eine Zahl ist
**/
bool _isNumber(char *s) {
  int i = strlen(s);
  while (i--) {
    if (s[i] > 47 && s[i] < 58)
      continue;
    else
      return false;
  }
  return true;
}

#endif
