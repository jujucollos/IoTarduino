#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#include <InfluxDbCloud.h>
#define INFLUXDB_URL "https://europe-west1-1.gcp.cloud2.influxdata.com/"
#define INFLUXDB_TOKEN "m8PkkqgADBq8p7xNaOlkMVnWxT7lzkUT5zQ_APe5Mz4D4RzthGK0-BBxYfCFxdyPrYvY56T-udVgLHPUgRxJrA=="
#define INFLUXDB_ORG "563de256efdb04f9"
#define INFLUXDB_BUCKET "MyData"

// WiFi AP SSID
#define WIFI_SSID "TCOM"
// WiFi password
#define WIFI_PASSWORD "<3Stephan<3"
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"



InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);


// Data point
Point sensor("wifi_status");

void setup() {
  Serial.begin(9600);
  
  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  // Add tags
  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WiFi.SSID());
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void loop() {
  // Store measured value into point
  sensor.clearFields();
  // Report RSSI of currently connected network
  sensor.addField("rssi", WiFi.RSSI());
  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(client.pointToLineProtocol(sensor));
  // If no Wifi signal, try to reconnect it
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  }
  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  float moisture = analogRead(34);
  Serial.print("Moisture: ");
  Serial.print(moisture);

  float water_level = analogRead(32);
  Serial.print("Water level: ");
  Serial.print(water_level);

  float humidity = analogRead(33);
  Serial.print("Humidity: ");
  Serial.print(humidity);

  Point pointDevice("sensor");
  pointDevice.addTag("device", DEVICE);
  pointDevice.addTag("SSID", WiFi.SSID());
  
  pointDevice.addField("Moisture Sensor Value",moisture);
  pointDevice.addField("Water level", water_level);
  pointDevice.addField("Humidity", humidity);
  
  Serial.print("Written to InfluxDB Cloud: ");
  Serial.println(client.writePoint(pointDevice));

  //Wait 5s
  Serial.println("Wait 5s");
  delay(5000);
}
