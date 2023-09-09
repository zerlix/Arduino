#ifndef SHELL_H
#define SHELL_H
#include <Arduino.h>

#define BUFFER_SIZE 64

class Shell
{

  public:
    Shell::Shell();
    bool Shell::getCommand();
    
  private:
    String sBuffer;
    int bSize = 0;

    bool Shell::execCommand();
    void Shell::_help();
    void Shell::_free();
    void Shell::_ls();
    
};
#endif
