
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

#define SERIAL_BAUDRATE 115200
#define DOWN	D1
#define UP		D2
#define STOP  D3

WiFiClient espClient;
PubSubClient client(espClient);

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void mqttSetup() {
    client.setServer(MQTT_SERVER, 1883);
    while (!client.connected()) {
        Serial.println("Connecting to MQTT...");

        if (client.connect("ESP8266Client", MQTT_USER, MQTT_PWD )) {

          Serial.println("connected");
          client.setCallback(callback);

        } else {

          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
        }
    }
    client.publish("esp/test", "Hello from ESP8266");
    client.subscribe("actor/shades");
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  payload[length] = '\0';
  char *cstring = (char *) payload;
  for (int i = 0; i < length; i++) {
  //  Serial.print((char)payload[i]);
  //  *original++ = payload[i];
  }
  Serial.println(cstring);
  Serial.println("-----------------------");

}

void setup() {
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("Starting...");

    // Wifi
    wifiSetup();
    mqttSetup();
}

void loop() {
    client.loop();
}
