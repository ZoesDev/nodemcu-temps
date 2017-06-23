#include <ESP8266WiFi.h>
#include <DHT.h>
#include <WiFiUdp.h>

#define DHTPIN 5
#define DHTTYPE DHT11
const char* ssid = "malware";
const char* password = "simlifesimlife";
const char* influxdb_host = "10.0.0.20";
const int influxdb_udp_port = 8888;

DHT dht(DHTPIN, DHTTYPE);

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.hostname("outside");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  dht.begin();
  Serial.println("Started DHT");

}

void loop() {
  float humidity = dht.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float temperature = dht.readTemperature(true);
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    String string_temperature = String(temperature);
    String string_humidity = String(humidity);
    String line = String("outside temperature=" + string_temperature + ",humidity=" + string_humidity);
    Serial.println(line);
    udp.beginPacket(influxdb_host, influxdb_udp_port);
    udp.print(line);
    udp.endPacket();
  }
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  delay(2000);
}