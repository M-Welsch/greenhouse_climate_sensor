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

void _checkUndervoltage() {
  if (status.batteryUndervoltage) {
    Serial.printf("Battery undervoltage. Going to deep sleep and check again in 1h. Vbat = %.2f", status.batteryVoltage);
    for (uint8_t c = 0; c < 10; c++) {
      led.flash();
      delay(50);
    }
    ESP.deepSleep(36e8);
  }
  else {
    Serial.printf("Battery Voltage Check passed: Vbat = %.2f\n", status.batteryVoltage);
  }
}

void setup()
{
  led.off();
  Serial.begin(115200);
  batteryMonitor.getVoltage(&status);
  _checkUndervoltage();
  dht.setup();
  led.setup(LED_BUILTIN);
  wifiConnection();
  mqttSetup();
}

void loop()
{
  reconnect();

  led.flash();
  dht.getValues(&status);
  batteryMonitor.getVoltage(&status);
  Serial.printf("T=%.2f, H=%.2f, Vb=%.2f\n", status.insideTemperature, status.insideHumidity, status.batteryVoltage);
  
  char buffer[256];

  sprintf(buffer, "{\"GewaechshausTemperatur\": %.2f, \"GewaechshausLuftfeuchtigkeit\": %.2f, \"GewaechshausBatterieSpannung\": %.2f}", status.insideTemperature, status.insideHumidity, status.batteryVoltage);
  mqttPublish(buffer);
  delay(100);
  //mqttPublishStatus(&status);
  ESP.deepSleep(60e6);  // reset after 60s
}
