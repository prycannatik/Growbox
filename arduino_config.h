// Pin-Definitionen
const int pHpin = A0;
const int ECpin = A1;
const int lightRelayPin = 7; // Relais für Lichtsteuerung
const int pumpDungerPin = 8; // Relais für Düngerpumpe
const int pumpWaterPin = 9; // Relais für Wasserzufuhr
const int pumpAcidPin = 10; // Relais für Säurezufuhr (Zitronenwasser)
const int buttonPinpH7 = 11; // Button für pH7-Kalibrierung
const int buttonPinpH4 = 12; // Button für pH4-Kalibrierung
const int measurementButtonPin = 13; // Button für pH-Messung

// OneWire Referenz für Temperatursensor
//OneWire oneWire(2);
//DallasTemperature sensors(&oneWire);

// Zielwerte für Salat
const float targetEC = 2.60; // in mS/cm
const float targetpHLow = 5.0;
const float targetpHHigh = 6.2;

// Lichtzyklus in Stunden
const long lightOnHours = 16;
const long lightOffHours = 8;
unsigned long lightChangeMillis = 0;
bool lightState = false;

// Kalibrierungsvariablen
float slope = -5.7; // Steigung für die pH-Wert Umrechnung
float intercept = 2.5; // Offset für die pH-Wert Umrechnung
bool calibrationComplete = false; // Status der Kalibrierung

// Gleitender Mittelwert für pH-Messung
const int numReadings = 10;
float readings[numReadings]; // speichert die Messwerte
int readIndex = 0; // aktueller Index im Array
float total = 0; // Summe der Messwerte

// Zustandsvariable für die Messung
bool measurementActive = false;

// Vorheriger Zustand des Messknopfes
bool prevMeasurementButtonState = HIGH;
