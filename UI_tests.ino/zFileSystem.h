#include <SdFat.h>
#include <SdFatConfig.h>


#if SPI_DRIVER_SELECT != 2
#error SPI_DRIVER_SELECT != 2
#endif


// SPI Pins
const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN = 13;
// Chip select Pin
const uint8_t SD_CS_PIN = 10;  // SD Karte


// Chipselect Pin Macro für SD Karten Slot definieren
#define SD_CS SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi)

// C++ Template, erstellt eine Klasse SoftSpiDriver
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;


SdFat sd;



class zFileSystem {


public:



  zFileSystem() {
    if (!sd.begin(SD_CS)) {
      Serial.println("Logger::Logger() - Kann SD Karte nicht ansprechen");
    }
  }




  bool mkdir(const char* dirName) {
    return sd.mkdir(dirName);
  }




  bool rmdir(const char* dirName) {
    return sd.rmdir(dirName);
  }



  uint32_t getFreeSpace() {
    uint32_t totalClusters = sd.clusterCount();
    uint32_t freeClusters = sd.freeClusterCount();
    uint32_t clusterSize = sd.bytesPerCluster();

    uint32_t freeSpace = freeClusters * clusterSize;
    return freeSpace / 1024 / 1024; // MB
  }



  void ls(const char* dirName) {
    SdFile file;
    if (!file.open(dirName, O_READ)) {
      Serial.println("Fehler beim Öffnen des Verzeichnisses.");
      return;
    }

    SdFile entry;
    while (entry.openNext(&file, O_READ)) {
      char filename[13];
      entry.getName(filename, sizeof(filename));
      Serial.println(filename);
      entry.close();
    }
    file.close();
  }




  SdFile create(const char* fileName) {
    SdFile file;
    if (file.open(fileName, O_WRITE | O_CREAT | O_TRUNC)) {
      return file;
    } else {
      Serial.println("Error creating file.");
      return SdFile();
    }
  }




  bool del(const char* fileName) {
    return sd.remove(fileName);
  }
};