#include "init.h"


File root;

FileSystem f;



// Command ls
void ls(int argCnt, char **args) {
  for (int i = 0; i < argCnt; i++) {
    Serial.print("Arg ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(args[i]);
  }
  root = SD.open("/");
  f.printDirectory(root, 0);
}



void setup() {

  // console starten
  cmd.begin(57600);


  // SD Card initialisieren
  while (!Serial)
    ;
  Serial.print("Initializing SD card...");
  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization failed!");
    while (1)
      ;
  }
  Serial.println("initialization done.");

  // commands festlegen
  cmd.add("ls", ls);
}



void loop() {
  // nothing happens after setup
  cmd.poll();
}
