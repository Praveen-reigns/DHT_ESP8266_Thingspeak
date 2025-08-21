#include <ESP8266WiFi.h>
#include "DHT.h"
#include "ThingSpeak.h"

// ---- WiFi Settings ----
const char* ssid = "IQOO";
const char* password = "1234567890";

// ---- ThingSpeak Settings ----
unsigned long myChannelNumber =  3042495;  
const char * myWriteAPIKey = "4TMBFA7SKURNT4KI";

// ---- DHT Settings ----
#define DHTPIN D4      // GPIO2 (D4 pin on NodeMCU)
#define DHTTYPE DHT11  // Use DHT22 if you have that sensor
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C, Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  // Send to ThingSpeak
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, h);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(20000); // ThingSpeak allows update every 15 sec minimum
}
