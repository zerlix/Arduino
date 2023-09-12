#include "shell.h"



Shell::Shell()
{
}


 
/**
 * liest commando von seriellen console,
 * speichert es in sBuffer und ruft _execCommand() auf
 */
bool Shell::getCommand()
{

  if(Serial.available() > 0) {
    sBuffer =  Serial.readStringUntil('\n');
    _execCommand();
  }
  return true;
}



/**
 * führt Methode entsprechend des sBuffer aus  
 **/
bool Shell::_execCommand()
{
  Serial.print("# ");
  Serial.println(sBuffer);

  if(sBuffer.equals("free")) {
    _free();
    return;
  }
  else if (sBuffer.equals("help")) {
    _help();
    return;
  } 
  else if (sBuffer.equals("ls")) {
    _ls();
    return;
  } 
  else {
    Serial.println("> Error: unbekannter Befehl.");
    _help();
    return;
  }
}



/**
 * "help" zeigt verfügbare Kommandos an
 */
void Shell::_help()
{
  Serial.println(F("> Verfügbare Kommandos: free, ls, help"));
}



/** 
 * Kommando "free" gibt den freiem dynamischem Speicher (Heap) auf der Seriellen Konsole aus
 */
void Shell::_free()
{
  extern int __heap_start, *__brkval;
  int v;
  Serial.print("> ");
  Serial.print((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
  Serial.println(" Bytes free");
}



/**
 * "ls" listet das Inhaltsverzeichnis der SD Karte auf
 */
void Shell::_ls()
{
  sd.ls(LS_R);
}
