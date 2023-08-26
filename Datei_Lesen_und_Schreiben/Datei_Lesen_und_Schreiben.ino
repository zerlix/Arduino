// SPI.h und SD.h werden für den Zugriff
// auf die SD Card benötigt
#include <SPI.h> 
#include <SD.h>

// CS Pin
// Kann sein das der Pin bei dir anders belegt ist
#define CS 10

// Erstelle ein Object myfile von der Klasse File
File myFile;

void setup() {
  // öffne serielle console zur Ausgabe
  Serial.begin(9600);
  while (!Serial) {
    ;  // Warten das die serielle Console bereit ist
  }



  Serial.println("Initialisiere SD card...");

  // Ist SD Card vorhanden  
  if (!SD.begin(CS)) {
    Serial.println("Initialisierung fehlgeschlagen!");
    while (1);
  }
  Serial.println("SD Card Initialisiert.");



  // Öffne die Datei zum Schreiben. 
  // Datei wird erstellt falls sie nicht existiert.
  // Beachten, dass jeweils nur eine Datei geöffnet werden kann.
  // Die Datei muss geschlossen werden, bevor ein andere geöffnet wird.
  // Wenn kein 2. Parameter angegeben wird die Datei zum Lesen geöffnet               
  myFile = SD.open("test.txt", FILE_WRITE);

  // Hat das öffnen/erstellen der Datei geklappt.
  if (myFile) {
    Serial.println("Schreibe test.txt...");
    // Datei schreiben und schliessen
    myFile.println("1, 2, 3."); // Inhalt in datei schreiben
    myFile.close();// schliesse die datei
    
    Serial.println("done.");
  } else {
    // fehlermeldung falls das öffnen der Datei nicht funktioniert hat
    Serial.println("error: Kann Datei nicht öffnen, test.txt");
  }




  // Öffne die Datei zum Lesen
  myFile = SD.open("test.txt");

  if (myFile) {
    Serial.println("Lese Datei test.txt:");

    // lese die datei zeichen, für zeichen ein und gebe sie aus.
    while (myFile.available()) {    // Beendet die schleife wenn die datei am Ende angekommen ist
      Serial.write(myFile.read());  // ausgabe der Datei (Seriele Console)
    }
    // Datei Schliessen
    myFile.close();
  } else {
    // Die Datei konnte nicht geöffnet werden
    Serial.println("error: Kann Datei nicht öffnen, test.txt");
  }
}


void loop() {
  // Nichts zu tun 
}
