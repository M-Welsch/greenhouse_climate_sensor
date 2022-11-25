#include <Arduino.h>
#include <string.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>

#include "dht22.h"
#include "status.h"
#include "mqtt.h"
#include "battery_monitor.h"
#include "led.h"

#define WIFI_HOTSPOT_NAME "Gewaechshaus_Sensor"

#define mqtt_server "192.168.0.2"
#define mqtt_user "iot"
#define mqtt_password "test123"

#define topic "Gewaechshaus"

status_t status;
BatteryMonitor batteryMonitor;
Dht dht;
Led led;

void wifiConnection() {
  WiFiManager wifiManager;
  wifiManager.autoConnect(WIFI_HOTSPOT_NAME);
}

void setup()
{
  Serial.begin(115200);
  dht.setup();
  led.setup(LED_BUILTIN);
  wifiConnection();
  mqttSetup();
}

uint16_t lastMsg = 0;
void loop()
{
  reconnect();

  uint16_t now = millis();
  if (now-lastMsg > 1000) {
    led.flash();
    dht.getValues(&status);
    Serial.printf("Ti: %.1f\n", status.insideTemperature);
    
    char buffer[256];
    sprintf(buffer, "{\"GewaechshausTemperatur\": %.2f, \"GewaechshausLuftfeuchtigkeit\": %.2f", status.insideTemperature, status.insideHumidity);
    mqttPublish(buffer);
    mqttPublishStatus(&status);
    delay(100);
  }
}
