#include "logger.h"


Logger L;

void setup() {
  
  Serial.begin(9600);
  while(!Serial);
  L.writeLog("test");
}

void loop() {


}

