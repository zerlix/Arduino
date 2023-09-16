#ifndef SHELL_H
#define SHELL_H
#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include "sensors.h"

extern SdFat sd;
extern Sensors s;   

/**
 * @class Shell
 * einfache Shell.
 * TODO:
 *     - command parser für argumente
 **/
class Shell
{

  public:
    Shell::Shell();  
    bool Shell::getCommand(); // überprüft Serielle konsole auf eingabe


  private:
    String sBuffer; // Buffer für input

    bool Shell::_execCommand(); // führt commando (sBuffer) aus
    void Shell::_help(); // commando help
    void Shell::_free(); // commando free
    void Shell::_ls();  // commando ls
    void Shell::_sensors(); // zeigt sensor daten an
    
};
#endif
