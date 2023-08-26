#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>


#define MINPRESSURE 50
#define MAXPRESSURE 1000


/**
 Um die Werte zu ermitteln den Beispiel Sketch, 
 TouchScreen_Calibr_native, von MCUFRIEND_KBV ausführen 
**/
// Sven´s Config
//const int TS_LEFT=179,TS_RT=904,TS_TOP=974,TS_BOT=200; // Touchpad Kalibrierung
//const int XP=9,XM=A3,YP=A2,YM=8; // Pinbelegung 

// Torsten´s Config
const int TS_LEFT=179,TS_RT=904,TS_TOP=974,TS_BOT=200;  // Touchpad Kalibrierung
const int XP=6,XM=A2,YP=A1,YM=7;  // Pinbelegung


// Erzeuge ein Object (tft) von der Klasse MCUFRIEND_kbv
MCUFRIEND_kbv tft;

// Erzeuge ein Object (ts) von der Klasse TouchScreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Erzeuge 4 Objekte/Buttons der Klasse Adafruit_GFX_Button
Adafruit_GFX_Button up_btn,
  down_btn,
  select_btn,
  back_btn;

// Globale Variable für Touch_getXY()
int pixel_x;
int pixel_y;

// Menu Positionszähler
int menu = 0;


/**
  function: MenuHeader()
  TODO: String übergeben der ausgegeben werden soll
  Gibt eine Überschrift / Titel aus
**/
void MenuHeader(void) {
  // ausgabe von Spumax V0.1
  tft.setCursor(100, 30);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.print("Spumax v0.1");
}

/**
  function void drawMenu ()
  erstellt und gibt das Hauptmenu aus
**/ 
void drawMenu() {
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(0, 0, 319, 240, TFT_WHITE);
  
  MenuHeader();


  // Menu Position korrigieren
  if (menu > 3) menu = 0;
  else if (menu < 0) menu = 3;

  tft.setCursor(20, 80);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(4);

  switch (menu) {
    case 0:  // Für menu=0 Displayausgabe Zeilen >1 und 2
      tft.print(">MenuItem1");
      tft.setCursor(20, 130);
      tft.print(" MenuItem2");
      break;

    case 1:  // Für menu=2 Displayausgabe Zeilen 1 und >2
      tft.print(" MenuItem1");
      tft.setCursor(20, 130);
      tft.print(">MenuItem2");
      break;

    case 2:  // Für menu=3 Displayausgabe Zeilen >3 und 4
      tft.print(">MenuItem3");
      tft.setCursor(20, 130);
      tft.print(" MenuItem4");
      break;

    case 3:  // Für menu=4 Displayausgabe Zeilen 3 und >4
      tft.print(" MenuItem3");
      tft.setCursor(20, 130);
      tft.print(">MenuItem4");
      break;
  }

  //ausgabe up Button
  up_btn.initButton(&tft, 40, 210, 70, 40, TFT_WHITE, TFT_RED, TFT_WHITE, "Up", 2);
  up_btn.drawButton(false);

  //ausgabe down Button
  down_btn.initButton(&tft, 120, 210, 70, 40, TFT_WHITE, TFT_RED, TFT_WHITE, "Down", 2);
  down_btn.drawButton(false);

  //ausgabe select Button
  select_btn.initButton(&tft, 210, 210, 80, 40, TFT_WHITE, TFT_RED, TFT_WHITE, "Select", 2);
  select_btn.drawButton(false);
}


/**
  function drawPage()
  erstellt und gibt die aufgerufene Seite aus.
**/
void drawPage() {

  bool z = true;

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 30);  //col , lin
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(4);
  
  // Welches Seite soll ausgegeben werden
  switch (menu) {
    case 0:
     tft.print("Seite 1");
     // Hier z.b. Sensor auslesen und werte ausgeben.
     // Bei längeren Code in Funktionen auslagern
     break;
    case 1:
     tft.print("Seite 2");
     // Hier z.b Sensor auslesen und werte ausgeben.
     // Bei längeren Code in Funktionen auslagern
     break;
    case 2:
     tft.print("Seite 3");
     // Hier z.b. Sensor auslesen und werte ausgeben.
     // Bei längeren Code in Funktionen auslagern
     break;
    case 3:
     tft.print("Seite 4");
     // Hier z.b. Sensor auslesen und werte ausgeben.
     // Bei längeren Code in Funktionen auslagern
     break;
  }  
  
  // Backbutton vorbereiten
  back_btn.initButton(&tft, 210, 210, 80, 40, TFT_WHITE, TFT_RED, TFT_WHITE, "Back", 2);
  back_btn.drawButton(false);
  
  // Warten das der Backbutton gedrückt wurde
  // z wird dann auf false gesetzt und somit die 
  // while schleife beendet und das Hauptmenu
  // neu erstellt und ausgegeben
  while (z) {
    
    bool down = Touch_getXY();
    back_btn.press(down && back_btn.contains(pixel_x, pixel_y));
    
    if (back_btn.justPressed()) {
      back_btn.drawButton(true);  //Button farbe ändern
      delay(200);
      z = false; // while beenden
      drawMenu(); // Hauptmenu neu zeichnen
    }
    delay(200);
  }

  return;
}


/**
  function: bool Touch_getXY(void)
  ermittelt ob das touchpad gedrückt wurde und speichert dann 
  die Coordinaten in den globalen Variablen, pixel_x und pixel_y, ab. 
**/
bool Touch_getXY(void) {
  TSPoint p = ts.getPoint();

  pinMode(YP, OUTPUT);  //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);  //because TFT control pins
  digitalWrite(XM, HIGH);

  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.y, TS_LEFT, TS_RT, 0, 320);
    pixel_y = map(p.x, TS_BOT, TS_TOP, 0, 240);
  }
  return pressed;
}


void setup() {

  // Display initialisieren
  tft.begin(tft.readID());
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  // Hauptmenu zeichnen
  drawMenu();
}



void loop() {
  bool down = Touch_getXY();

  // UP Button gedrückt ?
  up_btn.press(down && up_btn.contains(pixel_x, pixel_y));
  if (up_btn.justPressed()) {
    up_btn.drawButton(true);  //Button farbe ändern
    delay(500);
    menu--;  // menu um 1 runterzählen
    drawMenu();
  }
  if (up_btn.justReleased()) up_btn.drawButton();  // Button wurde losgelassen, farbe zurücksetzen

  // DOWN Button gedrückt ?
  down_btn.press(down && down_btn.contains(pixel_x, pixel_y));
  if (down_btn.justPressed()) {
    down_btn.drawButton(true);  //Button farbe ändern
    delay(500);
    menu++;  // menu um 1 hochzählen
    drawMenu();
  }
  if (down_btn.justReleased()) down_btn.drawButton();  // Button wurde losgelassen, farbe zurücksetzen

  // SELECT Button gedrückt ?
  select_btn.press(down && select_btn.contains(pixel_x, pixel_y));
  if (select_btn.justPressed()) {
    select_btn.drawButton(true);  //Button farbe ändern
    delay(500);
    drawPage();
  }
  if (select_btn.justReleased()) select_btn.drawButton();  // Button wurde losgelassen, farbe zurücksetzen
}


