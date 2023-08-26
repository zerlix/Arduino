///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////   I N I T I A L I S I E R U N G   ////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//LIBRARIES
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include <stdint.h>
#include <Adafruit_BMP085.h>

// BMP SENSOR AKTIVIEREN
Adafruit_BMP085 bmp;

//PINBELEGUNG FÜR DAS DISPLAY FESTLEGEN
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

//PINBELEGUNG FÜR DEN TOUCHSCREEN FESTLEGEN
#define YP A2
#define XM A3
#define YM 8
#define XP 9

//X- UND Y-GRENZEN FÜR DEN TOUCHSCREEN FESTLEGEN
#define TS_MINX 130
#define TS_MINY 110
#define TS_MAXX 950
#define TS_MAXY 930

//FARBDEFINITIONEN
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// BILDSCHIRMANSICHTEN VEREINBAREN
enum views{
  MENUVIEW,
  STANDBYVIEW
};

//DISPLAY EINRICHTEN
MCUFRIEND_kbv tft;

//EINFÜHREN UND VORBELEGEN VON VARIABLEN FÜR DIE SKALIERUNGSFAKTOREN DER SENSOREN
  double ScaleVol = 11700.0;        // Einführung der Variablen ScaleVol vom Typ double für Skalierung des Durchflussmessers
  double OffsetWa = 102.3;          // Einführung der Variablen OffsetWa vom Typ double für Nullpunktkorrektur des Wasserdrucks
  double ScaleWa = 8650.0/1023.0;   // Einführung der Variablen ScaleWal vom Typ double für Skalierung des Wasserdrucks
  double ScaleL1 = 193.0;           // Einführung der Variablen ScaleL1 vom Typ double für Skalierung des Luftdrucks an Messstelle 1
  double ScaleL2 = 193.0;           // Einführung der Variablen ScaleL2 vom Typ double für Skalierung des Luftdrucks an Messstelle 2

//EINFÜHREN VON VARIABLEN FÜR DEN DURCHFLUSSMESSER (DIGITALE MESSUNG)
  long TIMEOUT_WATER = 25000;           // Einführung der Variablen TimeOut vom Typ long für die Wartezeit nach der eine Sensorabfrage abgebrochen werden soll
  long DURATION_WATER_SENSOR = 50000;

// ZEILENABSTAND FÜR DIE MESSWERTANZEIGE IM STANDBYMODUS FESTLEGEN
  int STANDBYVIEW_YOFFSET = 20;

//FESTLEGEN VON PINS A13 bis A15 ALS INPUTS FÜR DIE DRUCKSENSOREN (ANALOGE MESSUNG)
  int SensorPinWa = A13;                   // Festlegung von Pin A13 als Analog-Input-Pin für die Druckmessung WASSER
  int SensorValueWa = 0L;                  // Vorbelegung der Variablen SensorValueWa mit der Zahl 0 als long-Variable
  int lastSensorValueWa = 0L;              // Vorbelegung der Variablen lastSensorValueWa mit der Zahl 0 als long-Variable

  int SensorPinL1 = A14;                   // Festlegung von Pin A14 als Analog-Input-Pin für die Druckmessung LUFT 1
  int SensorValueL1 = 0L;                  // Vorbelegung der Variablen SensorValueL1 mit der Zahl 0 als long-Variable
  int lastSensorValueL1 = 0L;              // Vorbelegung der Variablen lastSensorValueL1 mit der Zahl 0 als long-Variable

  int SensorPinL2 = A15;                   // Festlegung von Pin A15 als Analog-Input-Pin für die Druckmessung LUFT 2
  int SensorValueL2 = 0L;                  // Vorbelegung der Variablen SensorValueL2 mit der Zahl 0 als long-Variable
  int lastSensorValueL2 = 0L;              // Vorbelegung der Variablen lastSensorValueL2 mit der Zahl 0 als long-Variable

  int SENSOR_PIN_VOLUME_WATER = 35;        // Festlegung von Pin 35 als Digital-Input-Pin für die Durchflussmessung Wasser

// VORBELEGUNG VON WERTEN FÜR DIE BILDSCHIRMANSICHT
  bool viewChanged = true;
  int currentView = STANDBYVIEW;

// PINBELEGUNG UND WIDERSTAND IN OHM FÜR DEN TOUCHSCREEN FESTLEGEN
  TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   F U N K T I O N E N    //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TEXTAUSGABE
void printText(int xOffset, int yOffset, char* text, int textSize=3){
    tft.setTextColor(WHITE);
    tft.setTextSize(textSize);
    tft.setCursor(xOffset, yOffset);
    tft.print(text);
};

// WERTEAUSGABE
void printValue(const char* value, int rowIndex, bool marked, const char* unit){
      int xOffset = 120;
      int yOffset = STANDBYVIEW_YOFFSET+rowIndex*30;
      int xOffsetUnit = 260;
      int yOffsetUnit = STANDBYVIEW_YOFFSET + 6+rowIndex*30;
      
      if(marked) tft.fillRect(xOffset, yOffset, 130, 21, RED);   

      printText(xOffset, yOffset, value);
      
      if(unit=="") return;

      printText(xOffsetUnit, yOffsetUnit, unit, 2);
}

// VOLUMENZÄHLER WASSER EINLESEN
/**
* Belegung der Variablen `startTime` mit der aktuellen Zeit in Microsekunden
* Abfrage der Zeitdauer bis zum nächsten am Pin anliegenden Impuls
* und Abbruch dieser Abfrage nach Überschreitung der Wartezeit `TIMEOUT_WATER` größer ist als null.
* Wiederholung dieses Vorgangs bis die Torzeit `DURATION_WATER_SENSOR` verstrichen ist
* Berechnung des Volumenstroms A
* @return ausgelesenes Wasser Volumen
*/
double getWaterVolume(){
   int impulseCounter = 0;
    unsigned long startTime = micros();
    do{ if(pulseIn(SENSOR_PIN_VOLUME_WATER, HIGH, TIMEOUT_WATER) > 0) impulseCounter++;}
    while( micros() < (startTime + DURATION_WATER_SENSOR));
    double waterVolume = impulseCounter * 1e6/DURATION_WATER_SENSOR * (60.0/ScaleVol);
    return waterVolume;
}

// DRUCKMESSWERTE SKALIEREN
double scaleOffset(double value, double scale, double offset){
  return (value-offset)/scale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////             K L A S S E N              ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ABFRAGEBUTTON
class Button{
  public:
  int x;
  int y; 
  int w;
  int h;
  const char* text;
  Button(int x, int y, int w, int h, const char* text){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->text = text;
  }
  void draw(){
    tft.fillRect(x, y, w, h, RED);
    tft.drawRoundRect(x, y, w, h, 4, WHITE);
    printText(x+5,y+10,text,3);
  };

  bool isPressedBy(int posX, int posY){
    return (posX-x >= 0 && posX-x <= w && posY-y >= 0 && posY-y <= h);
  }
};

// STANDBYMODUS
class StandbyView{
  private:
    double prevVolume = 0;
    double prevWaterPressure = 0;
    double prevAir1 = 0;
    double prevAir2 = 0;
    double prevTemperature = 0;
    double prevAmbientPressure = 0;
    Button* menuButton;

  public:
    StandbyView(){
        menuButton = new Button(65, 200, 185, 40, "   MENU");
    }

    void display(){
      //BILDSCHIRM SCHWARZ EINFÄRBEN
      tft.fillScreen(BLACK);

      //TEXTAUSGABE
      tft.setCursor(10,STANDBYVIEW_YOFFSET);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("VOL = ");

      tft.setCursor(10,STANDBYVIEW_YOFFSET + 30*1);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("PWA = ");

      tft.setCursor(10,STANDBYVIEW_YOFFSET + 30*2);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("PL1 = ");

      tft.setCursor(10,STANDBYVIEW_YOFFSET + 30*3);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("PL2 = ");


      tft.setCursor(10,STANDBYVIEW_YOFFSET + 30*4);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("TMP = ");

      tft.setCursor(10,STANDBYVIEW_YOFFSET + 30*5);
      tft.setTextColor(WHITE);
      tft.setTextSize(3);
      tft.print("PUM = ");

      menuButton->draw();
        
      delay(20);
    }
    void loop(double volume, double waterPressure, double air1, double air2, double ambientPressure, double ambientTemperature, TSPoint *pressPointTft){
      
      //TEXTAUSGABE VOLUMENSTROM
      printValue(String(volume).c_str(), 0, volume!=this->prevVolume, "l/min");
      this->prevVolume = volume;

      //TEXTAUSGABE DRUCK WASSER
      if(abs(waterPressure-this->prevWaterPressure)>0.03*this->prevWaterPressure) {
          this->prevWaterPressure = 0.6*this->prevWaterPressure + 0.4*waterPressure;   // Tiefpassfilter
          printValue(String(this->prevWaterPressure).c_str(), 1, true, "mbar");
      }
      //TEXTAUSGABE DRUCK LUFT 1
      if(abs(air1-this->prevAir1)>0.03*this->prevAir1 || viewChanged){
          this->prevAir1 = 0.6*this->prevAir1 + 0.4*air1;   // Tiefpassfilter
          printValue(String(this->prevAir1).c_str(), 2, true, "mbar");  
      }
      //TEXTAUSGABE DRUCK LUFT 2
      if(abs(air2-this->prevAir2)>0.03*this->prevAir2 || viewChanged){
          this->prevAir2 = 0.6*this->prevAir2 + 0.4*air2;   // Tiefpassfilter
          printValue(String(this->prevAir2).c_str(), 3, true, "mbar");  
      }
      //TEXTAUSGABE TEMPERATUR
      if(abs(ambientTemperature-this->prevTemperature)>0.03*this->prevTemperature || viewChanged){
          this->prevTemperature = 0.6*this->prevTemperature + 0.4*ambientTemperature;   // Tiefpassfilter
          printValue(String(this->prevTemperature).c_str(), 4, true, "\xF7""C");
      }
      //TEXTAUSGABE UMGEBUNGSDRUCK      
      if(abs(ambientPressure-this->prevAmbientPressure)>0.03*this->prevAmbientPressure || viewChanged){
          this->prevAmbientPressure = 0.6*this->prevAmbientPressure + 0.4*ambientPressure;   // Tiefpassfilter
          printValue(String(prevAmbientPressure).c_str(), 5, true, "mbar");
      }

      viewChanged = false;

       if(pressPointTft != NULL && pressPointTft->z>0){ // screen is touched
        if(menuButton->isPressedBy(pressPointTft->x, pressPointTft->y)){
          viewChanged = true;
          currentView = MENUVIEW;
          //menuView->display();
        }
      }
    }
};

StandbyView* standbyView;

// MENÜMODUS
class MenuView{
  public:
  Button* betriebButton;
  Button* kalibrierungButton;
  Button* protokollausgabeButton;

  MenuView(){
    this->betriebButton = new Button(25, 30, 260,  40, "   BETRIEB");
    this->kalibrierungButton = new Button(25, 90, 260,  40, " KALIBRIERUNG");
    this->protokollausgabeButton = new Button(25, 150, 260, 40, "   PROTOKOLL");
  }

  void display(){
    tft.fillScreen(BLACK);
    betriebButton->draw();
    kalibrierungButton->draw();
    protokollausgabeButton->draw();
    delay(20);
  }

  void loop(double volume, double water, double air1, double air2, TSPoint *p){
    if(p != NULL && p->z>0){ // screen is touched
      if(betriebButton->isPressedBy(p->x, p->y)){
        viewChanged = true;
        currentView = STANDBYVIEW;
        standbyView->display();
      }
      else if(kalibrierungButton->isPressedBy(p->x, p->y)){
          //TODO change view to Kalibrierung
      }
      else if(protokollausgabeButton->isPressedBy(p->x, p->y)){
          //TODO change view to Protokollausgabe
      }
    }

  };
};

MenuView* menuView;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////   S E T U P   ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//START SETUPBEREICH
void setup() {
  pinMode(33, INPUT); //FESTLEGEN VON PIN 33 ALS INPUT FÜR DEN DURCHFLUSSMESSER

  tft.begin(tft.readID()); //DISPLAY INITIALISIEREN
  tft.setRotation(3); //BILDSCHIRMAUSRICHTUNG FESTLEGEN

  //display(); // Methode "display()" aufrufen 
  
  menuView = new MenuView();
  standbyView = new StandbyView();
  Serial.begin(9600);

  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
}

//ENDLOSSCHLEIFE ZUR INPUTABFRAGE UND AUSGABE
void loop() 
{
  TSPoint pressPointTft = ts.getPoint();  //Get touch point
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  
  ////ÜBERPRÜFEN OB DRUCKKRAFT GRÖSSER ALS 0 WAR UND FALLS JA:
  if (pressPointTft.z > 0) {
    //UMRECHNEN DER DRUCKPUNKTKOORDINATEN AUF DAS DISPLAYFORMAT
    pressPointTft.x = map(pressPointTft.x, TS_MAXX, TS_MINX, 0, 320);
    pressPointTft.y = map(pressPointTft.y, TS_MAXY, TS_MINY, 0, 240);
    pressPointTft.y = 240-pressPointTft.y; // flip y axis orientation
  }


  //ABFRAGE SENSOREN
  double waterVolume= getWaterVolume();
  SensorValueWa = analogRead(SensorPinWa);    //Abfrage Drucksensor Wasser
  SensorValueL1 = analogRead(SensorPinL1);    //Abfrage Drucksensor Luft 1
  SensorValueL2 = analogRead(SensorPinL2);    //Abfrage Drucksensor Luft 2


  // Auswertung
  double waterPressure = scaleOffset(SensorValueWa, ScaleWa, OffsetWa);
  double air1Pressure = scaleOffset(SensorValueL1, ScaleL1/1000, 0);
  double air2Pressure = scaleOffset(SensorValueL2, ScaleL2/1000, 0);
  double ambientPressure = bmp.readPressure()/100; 
  double ambientTemperature = bmp.readTemperature();

  switch(currentView){
    case STANDBYVIEW:
      if(viewChanged) {
        standbyView->display();
       };
      standbyView->loop(waterVolume, waterPressure, air1Pressure, air2Pressure, ambientPressure, ambientTemperature, &pressPointTft);
      break;
    case MENUVIEW:
      if(viewChanged) {
       menuView->display();
       viewChanged = false;
      };
      menuView->loop(waterVolume, waterPressure, air1Pressure, air2Pressure, &pressPointTft);
      break;
  }

  delay(50);  //WARTEZEIT BIS ZUM NEUEINSTIEG IN DIE ENDLOSSCHLEIFE
  
}




  
