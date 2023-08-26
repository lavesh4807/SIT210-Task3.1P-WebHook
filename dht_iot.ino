#include <DHT.h>
#include <WiFiNINA.h>

#include <ThingSpeak.h>

// Replace with your network credentials
const char* ssid = "laveshgarg";
const char* password = "lavesh3615";

// Replace with your ThingSpeak channel and API key
unsigned long channelID = 2251824;
const char* apiKey = "KHCJ2L5L6I0DBO86";

// Pin connected to DHT22 data pin
#define DHTPIN 2

// Uncomment the type of DHT sensor you're using
#define DHTTYPE DHT22 // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(115200);
 dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Read humidity value from DHT22 sensor
  // Read humidity and temperature values from DHT22 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // in Celsius

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Print humidity and temperature values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%   Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  // Update ThingSpeak channel with humidity and temperature values
  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperature);

  // Send the data to ThingSpeak
  int response = ThingSpeak.writeFields(channelID, apiKey);

  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP error code: ");
    Serial.println(response);
  }

  delay(20000); // Wait for 20 seconds before sending the next data
}



