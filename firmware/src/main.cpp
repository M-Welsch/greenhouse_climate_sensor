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

#define WIFI_HOTSPOT_NAME "mobilerKlimasensor"

#define FORCE_UPDATE_AFTER_X_BOOTUPS 10
#define FLASH_ADDRESS_ACTIVATIONS_WITHOUT_UPDATE 64

#define SIGNIFICANT_DIFFERENCE_TEMPERATURE 0.1
#define SIGNIFICANT_DIFFERENCE_HUMIDITY 0.1
#define SIGNIFICANT_DIFFERENCE_BATTERY_VOLTAGE 0.1
#define FLASH_ADDRESS_OLD_VALUES 4096

#define BUTTON 13

status_t status;
BatteryMonitor batteryMonitor;
Dht dht;
Led led;

void wifiConnection(bool openHotspot) {
  WiFiManager wifiManager;
  if (openHotspot) {
    Serial.printf("opening hotspot with ssid %s\n", WIFI_HOTSPOT_NAME);
    wifiManager.resetSettings();
    wifiManager.setConfigPortalTimeout(600);
    wifiManager.startConfigPortal(WIFI_HOTSPOT_NAME);
  }
  else {
    Serial.printf("Trying to connect to saved wifi network\n");
    wifiManager.setConfigPortalTimeout(1);      // basically don't open it at all ...
    wifiManager.setConnectTimeout(30);
    wifiManager.autoConnect(WIFI_HOTSPOT_NAME);
  }
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

bool _hasValueChanged(float oldVal, float newVal, float maxDiff) {
  bool hasChanged = abs(oldVal - newVal) > maxDiff;
  Serial.printf("old: %f, new: %f: %d\n", oldVal, newVal, hasChanged);
  return hasChanged;
}

bool _haveValuesChangedSignificantly(const status_t* const status) {
  status_t oldValues;
  EEPROM.begin(sizeof(status_t));
  EEPROM.get(FLASH_ADDRESS_OLD_VALUES, oldValues);
  Serial.printf("Last Values: T=%.2f, H=%.2f, Vb=%.2f\n", oldValues.insideTemperature, oldValues.insideHumidity, oldValues.batteryVoltage);
  bool tempChanged = _hasValueChanged(oldValues.insideTemperature, status->insideTemperature, SIGNIFICANT_DIFFERENCE_TEMPERATURE);
  bool humChanged = _hasValueChanged(oldValues.insideHumidity, status->insideHumidity, SIGNIFICANT_DIFFERENCE_HUMIDITY);
  bool vbatChanged = _hasValueChanged(oldValues.batteryVoltage, status->batteryVoltage, SIGNIFICANT_DIFFERENCE_BATTERY_VOLTAGE);
  return (tempChanged || humChanged || vbatChanged);
}

void _storeCurrentValues(const status_t* const status) {
  Serial.printf("Writing these to flash: T=%.2f, H=%.2f, Vb=%.2f\n", status->insideTemperature, status->insideHumidity, status->batteryVoltage);
  EEPROM.put(FLASH_ADDRESS_OLD_VALUES, status);
  EEPROM.commit();
  EEPROM.end();
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(BUTTON, INPUT_PULLUP);
  bool openHotspot = !digitalRead(BUTTON);
  led.setup(LED_BUILTIN);
  led.off();
  batteryMonitor.getVoltage(&status);
  _checkUndervoltage();
  dht.setup();
  dht.getValues(&status);
  //_haveValuesChangedSignificantly(&status);
  //_storeCurrentValues(&status);
  wifiConnection(openHotspot);
  mqttSetup();
}

void loop() {
  reconnect();

  led.flash();
  batteryMonitor.getVoltage(&status);
  Serial.printf("T=%.2f, H=%.2f, Vb=%.2f\n", status.insideTemperature, status.insideHumidity, status.batteryVoltage);
  
  char buffer[256];

  sprintf(buffer, "{\"mobilerKlimaSensorTemperatur\": %.2f, \"mobilerKlimaSensorLuftfeuchtigkeit\": %.2f, \"mobilerKlimaSensorBatterieSpannung\": %.2f, \"Random\": %ld}", status.insideTemperature, status.insideHumidity, status.batteryVoltage, random(255));
  mqttPublish(buffer);
  delay(100);
  //mqttPublishStatus(&status);
  ESP.deepSleep(60e6);  // reset after 1min
}
