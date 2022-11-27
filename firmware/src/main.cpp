#include <Arduino.h>
#include <string.h>

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <EEPROM.h>

#include "dht22.h"
#include "status.h"
#include "mqtt.h"
#include "battery_monitor.h"
#include "led.h"

#define WIFI_HOTSPOT_NAME "Gewaechshaus_Sensor"

#define FORCE_UPDATE_AFTER_X_BOOTUPS 10
#define FLASH_ADDRESS_ACTIVATIONS_WITHOUT_UPDATE 0

#define SIGNIFICANT_DIFFERENCE_TEMPERATURE 0.1
#define SIGNIFICANT_DIFFERENCE_HUMIDITY 0.1
#define SIGNIFICANT_DIFFERENCE_BATTERY_VOLTAGE 0.1
#define FLASH_ADDRESS_OLD_VALUES 2

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

bool _haveValuesChangedSignificantly(const status_t* const status) {
  status_t oldValues;
  EEPROM.begin(sizeof(oldValues));
  EEPROM.get(FLASH_ADDRESS_OLD_VALUES, oldValues);
  Serial.printf("Last Values: T=%.2f, H=%.2f, Vb=%.2f\n", oldValues.insideTemperature, oldValues.insideHumidity, oldValues.batteryVoltage);
  return (
    (abs(oldValues.insideTemperature - status->insideTemperature) > SIGNIFICANT_DIFFERENCE_TEMPERATURE) ||
    (abs(oldValues.insideHumidity - status->insideHumidity) > SIGNIFICANT_DIFFERENCE_HUMIDITY) ||
    (abs(oldValues.batteryVoltage - status->batteryVoltage) > SIGNIFICANT_DIFFERENCE_BATTERY_VOLTAGE)
  );
}

void _storeCurrentValues(const status_t* const status) {
  EEPROM.begin(sizeof(status_t));
  EEPROM.put(FLASH_ADDRESS_OLD_VALUES, status);
  EEPROM.commit();
}

void setup()
{
  led.setup(LED_BUILTIN);
  led.off();
  Serial.begin(115200);
  batteryMonitor.getVoltage(&status);
  _checkUndervoltage();
  dht.setup();
  dht.getValues(&status);
  //_haveValuesChangedSignificantly(&status);
  //_storeCurrentValues(&status);
  wifiConnection();
  mqttSetup();
}

void loop() {
  reconnect();

  led.flash();
  batteryMonitor.getVoltage(&status);
  Serial.printf("T=%.2f, H=%.2f, Vb=%.2f\n", status.insideTemperature, status.insideHumidity, status.batteryVoltage);
  
  char buffer[256];

  sprintf(buffer, "{\"GewaechshausTemperatur\": %.2f, \"GewaechshausLuftfeuchtigkeit\": %.2f, \"GewaechshausBatterieSpannung\": %.2f}", status.insideTemperature, status.insideHumidity, status.batteryVoltage);
  mqttPublish(buffer);
  delay(100);
  //mqttPublishStatus(&status);
  ESP.deepSleep(300e6);  // reset after 5mins
}
