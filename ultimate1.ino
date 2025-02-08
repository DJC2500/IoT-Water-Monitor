#include <OneWire.h>
#include <DallasTemperature.h>
#include <DFRobot_PH.h>

#define ONE_WIRE_BUS 7
#define TdsSensorPin A1
#define PH_PIN A2

int analogBuffer[30];
int analogBufferTemp[30];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0, temperature = 25;
float voltage, phValue;
float Celcius = 0;
float Fahrenheit = 0;
float volt, ntu;
DFRobot_PH ph;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode(TdsSensorPin, INPUT);
}

void loop() {
  volt = 0;

  for (int i = 0; i < 800; i++) {
    volt += ((float)analogRead(TdsSensorPin) / 1023) * 5;
  }

  volt = volt / 800;
  volt = round_to_dp(volt, 1);

  if (volt < 2.8) {
    ntu = 1540;
  } else {
    ntu = -576.12 * sq(volt) + 3393.2 * volt - 3443.2;
  }

  static unsigned long analogSampleTimepoint = millis();

  if (millis() - analogSampleTimepoint > 40U) {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);
    analogBufferIndex++;

    if (analogBufferIndex == 30)
      analogBufferIndex = 0;
  }

  static unsigned long timepoint = millis();

  if (millis() - timepoint > 1000U) {
    timepoint = millis();
    voltage = analogRead(PH_PIN) / 1024.0 * 5000;
    phValue = ph.readPH(voltage, temperature);
  }

  static unsigned long printTimepoint = millis();

  if (millis() - printTimepoint > 800U) {
    printTimepoint = millis();

    for (copyIndex = 0; copyIndex < 30; copyIndex++)
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];

    averageVoltage = getMedianNum(analogBufferTemp, 30) * 5.0 / 1024.0;
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
    float compensationVoltage = averageVoltage / compensationCoefficient;
    tdsValue = (133.42 * pow(compensationVoltage, 3) - 255.86 * pow(compensationVoltage, 2) + 857.39 * compensationVoltage) * 0.5;

    sensors.requestTemperatures();
    Celcius = sensors.getTempCByIndex(0);
    Fahrenheit = sensors.toFahrenheit(Celcius);

    String dataToSend = "pH:" + String(phValue, 2) + " TDS:" + String(tdsValue, 0) + " Turbidity:" + String(ntu, 0);
    Serial.println(dataToSend); // Send sensor data to the ESP32 over hardware serial

    Serial.print("C: ");
    Serial.print(Celcius);
    Serial.print(" F: ");
    Serial.print(Fahrenheit);
    Serial.print(" Volt: ");
    Serial.print(volt);

    Serial.print(" NTU: ");
    Serial.print(ntu);
    Serial.print(" pH: ");
    Serial.print(phValue);
    Serial.print(" TDS Value: ");
    Serial.print(tdsValue, 0);
    Serial.println(" ppm");
  }

  ph.calibration(voltage, temperature);

  delay(3000);
}

float round_to_dp(float in_value, int decimal_place) {
  float multiplier = pow(10.0, decimal_place);
  in_value = round(in_value * multiplier) / multiplier;
  return in_value;
}

int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;

  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }

  if (iFilterLen & 1)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;

  return bTemp;
}
