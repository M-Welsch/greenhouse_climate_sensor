#include "mqtt.h"

#define mqtt_server "192.168.0.2"
#define mqtt_user "iot"
#define mqtt_password "test123"

#define topic "Gewaechshaus"

WiFiClient espClient;
PubSubClient client(espClient);

void mqttSetup() {
    client.setServer(mqtt_server, 1883);   
}

void reconnect() {
  client.loop();
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

typedef enum {bsSUCCESS, bsERROR} bsError;

bsError wrapIntoJson(char* outstr, const char* instr) {
    if (instr==NULL || outstr==NULL) {
        return bsERROR;
    }
    sprintf(outstr, "{%s}", instr);
    return bsSUCCESS;
}

bsError jsonLine(char* outstr, const char* identifier, const float value, const bool lastLine) {
    if (identifier==NULL || outstr == NULL) {
        return bsERROR;
    }

    if (lastLine) {
        sprintf(outstr, "\"%s\": %f", identifier, value);
    } else {
        sprintf(outstr, "\"%s\": %f,", identifier, value);
    }
    return bsSUCCESS;
}

void mqttPublishStatus(status_t *status) {
  char buffer[512];
  buffer[0] = '\0';
  char tempBuffer[64];
  //sprintf(buffer, "{\"WaschkuecheTemperaturInnen\": %f}", status->insideTemperature, status->insideHumidity, status->insideDewPoint, status->outsideTemperature, status->outsideHumidity, status->outsideDewPoint);
  jsonLine(tempBuffer, "GewaechshausTemperatur", status->insideTemperature, false);
  sprintf(buffer, "%s%s", buffer, tempBuffer);
  jsonLine(tempBuffer, "GewaechshausLuftfeuchtigkeit", status->insideHumidity, false);
  sprintf(buffer, "%s%s", buffer, tempBuffer);
  jsonLine(tempBuffer, "GewaechshausTaupunkt", status->insideDewPoint, false);
  sprintf(buffer, "%s%s", buffer, tempBuffer);
  jsonLine(tempBuffer, "GewaechshausBatterieSpannung", status->batteryVoltage, true);
  sprintf(buffer, "%s%s", buffer, tempBuffer);
  wrapIntoJson(buffer, buffer);
  client.publish(topic, buffer, true);
}

void mqttPublish(char* buffer) {
    client.publish(topic, buffer, true);
}





// void sendToServer(const struct Status *status) {
//     connect();
//     char dataDictBuffer[256];
//     jsonLine(dataDictBuffer, "Ofentemperatur", status->probe_temperature, false);
//     jsonLine(dataDictBuffer, "Aussentemperatur", status->ambient_temperature, false);
//     jsonLine(dataDictBuffer, "Batteriespannung", status->battery_voltage, true);

//     char dataDict[256];
//     wrapIntoJson(dataDict, dataDictBuffer);
//     mqttClient.publish(mqtt_topic, dataDict, true);
// }