#include <stdlib.h>

#ifndef SENSORS_H
#define SENSORS_H
#include <stdio.h>
#include <Adafruit_BMP085.h>

/** TODO:
  * Klasse muss überarbeitet werden
  **/  

class Sensors {


public:
  
  double waterVolume;
  double waterPressure;
  double air1Pressure;
  double air2Pressure;
  double ambientPressure;
  double ambientTemperature;


  //EINFÜHREN UND VORBELEGEN VON VARIABLEN FÜR DIE SKALIERUNGSFAKTOREN DER SENSOREN
  double ScaleVol = 11700.0;         // Einführung der Variablen ScaleVol vom Typ double für Skalierung des Durchflussmessers
  double OffsetWa = 102.3;           // Einführung der Variablen OffsetWa vom Typ double für Nullpunktkorrektur des Wasserdrucks
  double ScaleWa = 8650.0 / 1023.0;  // Einführung der Variablen ScaleWal vom Typ double für Skalierung des Wasserdrucks
  double ScaleL1 = 193.0;            // Einführung der Variablen ScaleL1 vom Typ double für Skalierung des Luftdrucks an Messstelle 1
  double ScaleL2 = 193.0;            // Einführung der Variablen ScaleL2 vom Typ double für Skalierung des Luftdrucks an Messstelle 2


  //FESTLEGEN VON PINS A13 bis A15 ALS INPUTS FÜR DIE DRUCKSENSOREN (ANALOGE MESSUNG)
  int SensorPinWa = A13;       // Festlegung von Pin A13 als Analog-Input-Pin für die Druckmessung WASSER
  int SensorValueWa = 0L;      // Vorbelegung der Variablen SensorValueWa mit der Zahl 0 als long-Variable
  int lastSensorValueWa = 0L;  // Vorbelegung der Variablen lastSensorValueWa mit der Zahl 0 als long-Variable

  int SensorPinL1 = A14;       // Festlegung von Pin A14 als Analog-Input-Pin für die Druckmessung LUFT 1
  int SensorValueL1 = 0L;      // Vorbelegung der Variablen SensorValueL1 mit der Zahl 0 als long-Variable
  int lastSensorValueL1 = 0L;  // Vorbelegung der Variablen lastSensorValueL1 mit der Zahl 0 als long-Variable

  int SensorPinL2 = A15;       // Festlegung von Pin A15 als Analog-Input-Pin für die Druckmessung LUFT 2
  int SensorValueL2 = 0L;      // Vorbelegung der Variablen SensorValueL2 mit der Zahl 0 als long-Variable
  int lastSensorValueL2 = 0L;  // Vorbelegung der Variablen lastSensorValueL2 mit der Zahl 0 als long-Variable

  int SENSOR_PIN_VOLUME_WATER = 35;  // Festlegung von Pin 35 als Digital-Input-Pin für die Durchflussmessung Wasser

  //EINFÜHREN VON VARIABLEN FÜR DEN DURCHFLUSSMESSER (DIGITALE MESSUNG)
  long TIMEOUT_WATER = 25000;  // Einführung der Variablen TimeOut vom Typ long für die Wartezeit nach der eine Sensorabfrage abgebrochen werden soll
  long DURATION_WATER_SENSOR = 50000;
 
  Adafruit_BMP085 bmp; //bmp Sensor
  const char CSVbuffer[64];

  Sensors() {
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
  double Sensors::getWaterVolume() {

    int impulseCounter = 0;
    unsigned long startTime = micros();

    do {
      if (pulseIn(SENSOR_PIN_VOLUME_WATER, HIGH, TIMEOUT_WATER) > 0)
        impulseCounter++;
    } while (micros() < (startTime + DURATION_WATER_SENSOR));

    waterVolume = impulseCounter * 1e6 / DURATION_WATER_SENSOR * (60.0 / ScaleVol);

    return waterVolume;
  }





  // DRUCKMESSWERTE SKALIEREN
  double scaleOffset(double value, double scale, double offset) {
    return (value - offset) / scale;
  }





  void Sensors::getSensorData() {

    //ABFRAGE SENSOREN
    waterVolume = getWaterVolume();
    SensorValueWa = analogRead(SensorPinWa);  //Abfrage Drucksensor Wasser
    SensorValueL1 = analogRead(SensorPinL1);  //Abfrage Drucksensor Luft 1
    SensorValueL2 = analogRead(SensorPinL2);  //Abfrage Drucksensor Luft 2

    
    // Auswertung
    waterPressure = scaleOffset(SensorValueWa, ScaleWa, OffsetWa);
    air1Pressure = scaleOffset(SensorValueL1, ScaleL1 / 1000, 0);
    air2Pressure = scaleOffset(SensorValueL2, ScaleL2 / 1000, 0);



    /************** ACHTUNG ***********************/
    /** Blockiert die Ausführung wenn Sensor nicht angesprochen werden kann*/
    // ambientPressure = bmp.readPressure()/100; 
    // ambientTemperature = bmp.readTemperature();
    /***/

    // Dummy Werte, wegen fehlenden Sensoren
    waterVolume = 1000.37;
    waterPressure = 2000.00;
    air1Pressure = 1400.24;
    air2Pressure = 3200.35;
    ambientPressure = 100.00;
    ambientTemperature = 30.00;
  }



   
  // Gibt eine zeile csv formatierte Sensordaten zurück
  char* Sensors::getCsvSensorData() {
    
    // Sensordaten aktualisieren
    getSensorData();
    
    char wv[8];
    char wp[8];
    char ap1[8];
    char ap2[8];
    char p[8];
    char t[8];

    dtostrf(waterVolume, 6, 2, wv);
    dtostrf(waterPressure, 6, 2, wp);
    
    dtostrf(air1Pressure, 5, 2, ap1);
    dtostrf(air2Pressure, 6, 2, ap2);
    
    dtostrf(ambientPressure, 6, 2, p);
    dtostrf(ambientTemperature, 6, 2, t);
    
    // Sensordaten formatieren (0,0,23567,16987,15110,17687)

    sprintf(CSVbuffer, "%s,%s,%s,%s,%s,%s", wv, wp, ap1, ap2, p, t);
   
        // Umwandlung und Formatierung direkt in CSVbuffer
    //snprintf(CSVbuffer, sizeof(CSVbuffer), "%4.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f",
      //       waterVolume, waterPressure, air1Pressure, air2Pressure, ambientPressure, ambientTemperature);
    
    return CSVbuffer;
  }
};

#endif