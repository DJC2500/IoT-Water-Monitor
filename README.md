# IoT-Based Data Acquisition System for Monitoring Water Quality in First Nation Reserves

This project utilizes an ESP32 microcontroller to collect and transmit real-time water quality data to ThingSpeak for cloud-based monitoring and analysis.

## Features
- Establishes Wi-Fi connectivity with ESP32
- Collects data from multiple water quality sensors (pH, TDS, NTU, Turbidity, Temperature, Voltage)
- Parses and processes incoming sensor data
- Transmits real-time data to ThingSpeak for remote monitoring
- Provides serial output for debugging and data verification

## Components Required
- ESP32 Development Board
- pH Sensor
- TDS Sensor
- Turbidity Sensor
- Temperature Sensor

## Circuit Diagram
Connect the sensors to the ESP32's analog and digital pins as per their specifications.

## Installation
1. Install [Arduino IDE](https://www.arduino.cc/en/software) if not already available.
2. Add ESP32 board support to the Arduino IDE.
3. Install the required libraries:
   - `WiFi.h` (Built-in for ESP32)
   - `HTTPClient.h` (Built-in for ESP32)
4. Clone this repository or copy the provided code into a new Arduino sketch.
5. Configure the following credentials in the code:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   const char* apiKey = "YOUR_THINGSPEAK_API_KEY";
   ```
6. Upload the code to your ESP32 module.

## Usage
1. Power up the ESP32 module.
2. Open the Serial Monitor (115200 baud rate).
3. Ensure successful Wi-Fi connection.
4. Transmit sensor data via the Serial Monitor or connected sensor modules.
5. View real-time water quality metrics on ThingSpeak.

## Expected Serial Output
```
Connecting to WiFi...
Connected to WiFi
Received data: C:25.5 F:77.9 Volt:3.2 NTU:15 pH:7.2 TDS Value:450 ppm
C: 25.5
F: 77.9
Volt: 3.2
NTU: 15
pH: 7.2
TDS Value: 450
Sending data to ThingSpeak. URL: https://api.thingspeak.com/update?api_key=...
HTTP Response code: 200
```

## ThingSpeak Integration
The ESP32 sends collected sensor data to ThingSpeak using HTTP requests. Ensure the ThingSpeak channel is configured with the following field mappings:
- **Field 1**: pH Level
- **Field 2**: TDS Value
- **Field 3**: NTU (Turbidity)
- **Field 4**: Turbidity (Alternative)
- **Field 5**: Temperature (°C)
- **Field 6**: Temperature (°F)
- **Field 7**: Voltage

## Troubleshooting
- **Wi-Fi Not Connecting**: Verify the SSID and password.
- **No Serial Output**: Ensure the Serial Monitor baud rate is set correctly.
- **Data Not Appearing on ThingSpeak**: Check the API key and channel settings.

## Video Presentation:
Video: https://murf.ai/share/lpiiwrl2

## Contribution:
Electronics III Final Project from the University of Ottawa.

## Author
Developed by Divine Joseph Ciroma 

