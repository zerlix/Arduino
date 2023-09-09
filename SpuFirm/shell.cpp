#include "shell.h"


Shell::Shell()
{
}


bool Shell::getCommand()
{

  if(Serial.available() > 0) {
    sBuffer =  Serial.readStringUntil('\n');
    Serial.println(sBuffer);
    execCommand();
  }

  
  return true;
}



bool Shell::execCommand()
{
  if(sBuffer.equals("free")) {
    _free();
  } 
  else if (sBuffer.equals("ls")) {
    _ls();
  } 
  else {
    _help();
  }
}



void Shell::_help()
{
  Serial.println(F("Verfügbare Kommandos:"));
  Serial.println(F("free"));
  Serial.println(F("ls"));
}



void Shell::_free()
{
  extern int __heap_start, *__brkval;
  int v;
  Serial.print("> ");
  Serial.print((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
  Serial.println(" Bytes free");
}


void Shell::_ls()
{
  int i = 0;
}
