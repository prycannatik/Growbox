#include <OneWire.h>
#include <arduino_config.h>
//#include <DallasTemperature.h>

void setup() {
    Serial.begin(9600);
    pinMode(lightRelayPin, OUTPUT);
    pinMode(pumpDungerPin, OUTPUT);
    pinMode(pumpWaterPin, OUTPUT);
    pinMode(pumpAcidPin, OUTPUT);
    pinMode(buttonPinpH7, INPUT_PULLUP);
    pinMode(buttonPinpH4, INPUT_PULLUP);
    pinMode(measurementButtonPin, INPUT_PULLUP);
    //sensors.begin();
    lightChangeMillis = millis();
    
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
        readings[thisReading] = 0; // Initialisiere das Array mit 0
    }
    // Zusätzlicher Code zum direkten Einschalten des Wasser-Relais beim Start
    digitalWrite(pumpWaterPin, HIGH); // Wasser-Relais einschalten
}

void calibratepH(int point) {
    // Lesen Sie die aktuelle Spannung vom pH-Sensor
    float currentVoltage = analogRead(pHpin) * (5.0 / 1023.0);
    Serial.print("Aktuelle Spannung für Kalibrierpunkt ");
    Serial.print(point);
    Serial.print(": ");
    Serial.println(currentVoltage, 3);

    // Wenn der Kalibrierungspunkt pH 7 ist, setzen wir den Offset auf den gewünschten Wert,
    // um den pH-Wert auf 7 zu korrigieren.
    if (point == 7) {
        // Offset Korrektur
        intercept = 7.0 - (currentVoltage * slope);
        Serial.print("pH-Wert auf 7.0 kalibriert. Neuer Offset: ");
        Serial.println(intercept, 3);
    }

    // Der Kalibrierungsstatus wird auf "abgeschlossen" gesetzt,
    // was in diesem Kontext bedeutet, dass der pH-Wert auf 7.0 festgesetzt ist.
    calibrationComplete = true;
}

float getMovingAverage(float newReading) {
    total = total - readings[readIndex];
    readings[readIndex] = newReading;
    total = total + newReading;
    readIndex = readIndex + 1;
    
    if (readIndex >= numReadings) {
        readIndex = 0;
    }
    return total / numReadings;
}

void regulatepH(float pH) {
    static unsigned long lastAcidPumpMillis = 0;
    if (pH > targetpHHigh && millis() - lastAcidPumpMillis > 60000) { // 60 Sekunden Wartezeit
        digitalWrite(pumpAcidPin, HIGH);
        delay(1000); // Säure für eine kurze Zeit zuführen
        digitalWrite(pumpAcidPin, LOW);
        lastAcidPumpMillis = millis();
    }
}

void loop() {
    if ((lightState && millis() - lightChangeMillis >= lightOnHours * 3600000UL) ||
        (!lightState && millis() - lightChangeMillis >= lightOffHours * 3600000UL)) {
        lightState = !lightState;
        digitalWrite(lightRelayPin, lightState ? HIGH : LOW);
        lightChangeMillis = millis();
    }

    // Lesen Sie den aktuellen Zustand des Messknopfes
    bool currentMeasurementButtonState = digitalRead(measurementButtonPin);

    // Wenn der Knopf für die Messung gedrückt wird und der vorherige Zustand nicht gedrückt war,
    // ändern Sie den Zustand der Messung
    if (currentMeasurementButtonState == LOW && prevMeasurementButtonState == 0) {
        measurementActive = !measurementActive;
        // Wenn die Messung deaktiviert wird, schalten Sie alle Pumpen aus
        if (!measurementActive) {
            digitalWrite(pumpDungerPin, LOW);
            digitalWrite(pumpAcidPin, LOW);
        }
        // Ausgabe in der Konsole je nachdem, ob die Messung aktiviert oder deaktiviert wurde
        Serial.println("Messung deaktiviert.");
        prevMeasurementButtonState = 1;
    }
    else if (currentMeasurementButtonState == LOW && prevMeasurementButtonState == 1) {
        measurementActive = !measurementActive;
        // Wenn die Messung aktiviert wird, schalten Sie alle Pumpen an
        if (!measurementActive) {
            digitalWrite(pumpDungerPin, HIGH);
            digitalWrite(pumpAcidPin, HIGH);
        }
        // Ausgabe in der Konsole je nachdem, ob die Messung aktiviert oder deaktiviert wurde
        Serial.println("Messung aktiviert.");
        prevMeasurementButtonState = 0;
    }

    // Wenn die Messung aktiv ist, führen wir die pH-Messung und -Regulierung durch
    if (measurementActive) {
        if (digitalRead(buttonPinpH7) == LOW) {
            calibratepH(7);
        } else if (digitalRead(buttonPinpH4) == LOW) {
            calibratepH(4);
        }

        float voltage = analogRead(pHpin) * (5.0 / 1023.0);
        float pH = getMovingAverage(voltage) * slope + intercept;
        Serial.print("Geglätter pH-Wert: ");
        Serial.println(pH, 2);
        regulatepH(pH);

        int sensorValueEC = analogRead(ECpin);
        float voltageEC = sensorValueEC * (5.0 / 1023.0);
        float EC = voltageEC; // Kalibrierung notwendig
        Serial.print("EC-Wert: ");
        Serial.println(EC, 2);

        if (EC < targetEC) {
            digitalWrite(pumpDungerPin, HIGH);
            delay(1000); // Dünger für eine kurze Zeit zuführen
            digitalWrite(pumpDungerPin, LOW);
        }
    }

    delay(1000); // Wartezeit von 1 Sekunde zwischen den Messungen
}
