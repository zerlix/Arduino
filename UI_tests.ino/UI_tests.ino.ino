#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "zFileSystem.h"
//#include <Adafruit_ImageReader.h>

//MCUFRIEND_kbv tft;  // tft object
#define TFT_DC 9
#define TFT_CS 10

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

uint16_t ID;        // Display Typ

zFileSystem sdCard; 

//Adafruit_ImageReader reader(SD);



void setup() {

  Serial.begin(57600);
  while (!Serial)
    ;

  //tft.reset();
  //ID = tft.readID();
  //tft.begin(ID);
  tft.begin();
  
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  displayInfo();

  sdCard.create("test.txt");
  sdCard.ls("/");
  Serial.println(sdCard.getFreeSpace());
}





void loop() {
}







void displayInfo() {

  int rotation = tft.getRotation();
  int height = tft.height();
  int width = tft.width();

  Serial.print("TFT Typ:  0x");
  Serial.println(ID, HEX);


  if (rotation % 2 == 0) {
    Serial.print("Auflösung: ");
    Serial.print(height);
    Serial.print("x");
    Serial.println(width);
    Serial.print("Rotation: ");
    Serial.print("Portrait - ");
  } else {
    Serial.print("Auflösung: ");
    Serial.print(width);
    Serial.print("x");
    Serial.println(height);
    Serial.print("Rotation: ");
    Serial.print("Landscape - ");
  }
  Serial.println(rotation);
}






