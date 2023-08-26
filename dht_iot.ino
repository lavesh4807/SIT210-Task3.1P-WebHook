#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#define DHTPIN 2
#define DHTTYPE DHT22 // DHT 22 (AM2302)

const char* ssid = "laveshgarg";
const char* password = "lavesh3615";
unsigned long channelID = 2251824;
const char* apiKey = "KHCJ2L5L6I0DBO86";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  ThingSpeak.begin(client);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // in Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%   Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);

  int response = ThingSpeak.writeFields(channelID, apiKey);

  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP error code: ");
    Serial.println(response);
  }

  delay(20000);
}



