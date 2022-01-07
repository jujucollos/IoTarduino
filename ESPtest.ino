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
#include "secrets.h" // load connection credentials

#define SLEEP_S 30 // how many seconds to sleep between readings
#define DEVICE_ID "myroom"

// InfluxDB client for InfluxDB Cloud API
InfluxDBClient client_cloud(INFLUXDB_CLOUD_URL, INFLUXDB_CLOUD_ORG, INFLUXDB_CLOUD_BUCKET, INFLUXDB_CLOUD_TOKEN, InfluxDbCloud2CACert);
// InfluxDB client instance for self-hosted v1 database
InfluxDBClient client_v1;

// because we use the deepsleep pattern we put all our logic in the setup() routine and then send the microcontroller back to sleep
//  if your microcontroller is always up you might want to put the commands in your loop() routine instead
void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup");
  delay(100);
  wifiConnect(WIFI_SSID, WIFI_KEY);
  // BEGIN: read sensor values (add your real code here)
  float sensor_value1 = 1.23;
  float sensor_value2 = 0;
  // END: read sensor values

  Point pointDevice("mymeasurement"); // create a new measurement point (the same point can be used for Cloud and v1 InfluxDB)
  // add tags to the datapoints so you can filter them
  pointDevice.addTag("device", DEVICE_ID);
  pointDevice.addTag("SSID", WiFi.SSID());
  // Add data fields (values)
  pointDevice.addField("sensor1", sensor_value1);
  pointDevice.addField("sensor2", sensor_value2);
  pointDevice.addField("uptime", millis()); // in addition send the uptime of the Arduino
  
  Serial.print("written to InfluxDB Cloud: ");
  Serial.println(client_cloud.writePoint(pointDevice)); // returns true if success, false otherwise

  client_v1.setConnectionParamsV1(INFLUXDB_URL, INFLUXDB_DATABASE, INFLUXDB_USER, INFLUXDB_PASSWORD);
  Serial.print("written to local InfluxDB instance: ");
  Serial.println(client_v1.writePoint(pointDevice)); // returns true if success, false otherwise

  // this sends the microcontroller to deepsleep until the next reading needs to be taken
  //  due to WiFi connect and sensor reading your measurement interval will always be 5~10 seconds longer than the SLEEP_S duration
  ESP.deepSleep(SLEEP_S * 1000000 - millis()*1000); // offset by the duration the program run (converted from ms to µs)
}

void loop() {
}

// try to connect to given SSID and key, loop until successful
void wifiConnect(const char* ssid, const char* key) {
  WiFi.begin(ssid, key);
  Serial.print("Waiting for WiFi connection..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(".");
  Serial.print("Successfully connected to ");
  Serial.println(WiFi.SSID());
}
