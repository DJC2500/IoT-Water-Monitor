#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Durkio";
const char* password = "mylife20";
const char* apiKey = "J0JTL1YWSU5WE2WZ";

void setup() {
  Serial.begin(115200);
  delay(10);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    Serial.print("Received data: ");
    Serial.println(data);

    float pH, TDS, NTU, Turbidity, C, F, Volt;

    // Attempt to parse the data with all fields
    if (sscanf(data.c_str(), "C:%f F:%f Volt:%f NTU:%f pH:%f TDS Value:%f ppm", &C, &F, &Volt, &NTU, &pH, &TDS) == 6) {
      Serial.print("C: ");
      Serial.println(C);
      Serial.print("F: ");
      Serial.println(F);
      Serial.print("Volt: ");
      Serial.println(Volt);
      Serial.print("NTU: ");
      Serial.println(NTU);
      Serial.print("pH: ");
      Serial.println(pH);
      Serial.print("TDS Value: ");
      Serial.println(TDS);

      // Proceed to send the data to ThingSpeak or perform other actions as needed.
      sendDataToThingSpeak(pH, TDS, NTU, Turbidity, C, F, Volt);
    }
    // If the above format parsing fails, try parsing the data with the subset of fields
    else if (sscanf(data.c_str(), "pH:%f TDS:%f Turbidity:%f", &pH, &TDS, &Turbidity) == 3) {
      Serial.print("pH: ");
      Serial.println(pH);
      Serial.print("TDS: ");
      Serial.println(TDS);
      Serial.print("Turbidity: ");
      Serial.println(Turbidity);

      // Proceed to send the data to ThingSpeak or perform other actions as needed.
      sendDataToThingSpeak(pH, TDS, NTU, Turbidity, C, F, Volt);
    } else {
      Serial.println("Data format does not match expected format.");
    }
  }
}

void sendDataToThingSpeak(float pH, float TDS, float NTU, float Turbidity, float C, float F, float Volt) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = "https://api.thingspeak.com/update?api_key=";
    url += apiKey;
    url += "&field1=" + String(pH, 2);   // Send pH with 2 decimal places
    url += "&field2=" + String(TDS, 0);  // Send TDS as an integer
    url += "&field3=" + String(NTU, 0);  // Send NTU as an integer
    url += "&field4=" + String(Turbidity, 0);  // Send Turbidity as an integer
    url += "&field5=" + String(C, 2);  // Send C with 2 decimal places
    url += "&field6=" + String(F, 2);  // Send F with 2 decimal places
    url += "&field7=" + String(Volt, 2);  // Send Volt with 2 decimal places

    Serial.print("Sending data to ThingSpeak. URL: ");
    Serial.println(url);

    http.begin(url);

    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  delay(3000); // Update ThingSpeak every 5 seconds (you can adjust this interval)
}
