#ifndef FileSystem_h
#define FileSystem_h

#include <SPI.h>
#include <SD.h>



class FileSystem : File {
  private :
    String name;

  public :
    void printDirectory(File dir, int numTabs);
    int mkDir(String name);
};



#endif