#if defined(ESP32)
  #include <WiFiMulti.h>
  WiFiMulti wifiMulti;
  #define DEVICE "ESP32"
#elif defined(ESP8266)
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
#endif
#include <InfluxDbClient.h> // load the client library
#include <InfluxDbCloud.h> // only for InfluxDB Cloud: load SSL certificate and additional method call
#include "secrets.h"

#define SLEEP_S 30
#define DEVICE_ID "MyRoom"

// InfluxDB client for InfluxDB Cloud API
InfluxDBClient client_cloud(INFLUXDB_CLOUD_URL, INFLUXDB_CLOUD_ORG, INFLUXDB_CLOUD_BUCKET, INFLUXDB_CLOUD_TOKEN, InfluxDbCloud2CACert);
// InfluxDB client instance for self-hosted v1 database
InfluxDBClient client;

void wifiConnect(const char* ssid, const char* key) {
  WiFi.begin(ssid, key);
  Serial.print("Waiting for WiFi connection..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }
  Serial.println(".");
  Serial.print("Successfully connected to ");
  Serial.println(WiFi.SSID());

}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup");
  delay(100);
  wifiConnect(WIFI_SSID, WIFI_KEY);
  pinMode(led, OUTPUT);
}

void loop() {

  int val_temp_linear;
  int dat_temp_linear;
  
  Serial.print("Moisture Sensor Value:");
  sensor_moisture = analogRead(0);
  sensor_water_level = analogRead(1);
  Serial.println(sensor_moisture);
  Serial.print("Water Level Value:");
  Serial.println(sensor_water_level);
  tmp_temperature = analogRead(2);//Connect LM35 on Analog 2
  sensor_temperature = (500 * tmp_temperature) /1024;
  
  Point pointDevice("sensor");
  pointDevice.addTag("device", DEVICE_ID);
  pointDevice.addTag("SSID", WiFi.SSID());
  
  pointDevice.addField("Moisture Sensor Value", sensor_moisture);
  pointDevice.addField("Water level", sensor_water_level);
  pointDevice.addField("Temperature", sensor_temperature);
  
  Serial.print("Written to InfluxDB Cloud: ");
  Serial.println(client_cloud.writePoint(pointDevice));

  client.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DATABASE, INFLUXDB_USER, INFLUXDB_PASSWORD);
  Serial.print("Written to local InfluxDB instance: ");
  Serial.println(client.writePoint(pointDevice));

  ESP.deepSleep(SLEEP_S * 1000000 - millis()*1000); // offset by the duration the program run (converted from ms to µs)
}
