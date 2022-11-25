#include "web_server.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <string.h>
#include "status.h"
#include "table.h"
#include "json.h"

Status *webserver_status;

ESP8266WebServer server(80);

void handle_root(void) {
    String s = HEAD;
    s.concat(concat_data_table(webserver_status));
    s.concat(FOOT);
    server.send(200, "text/html", s);
}

void handle_rest_api(void) {
    String s = concat_json(webserver_status);
    server.send(200, "application/json", s);
}

void WebServer::handle_clients(void) {
    server.handleClient();
}

void WebServer::setup(Status *main_status) {
    WiFi.begin(ssid, password);
    server.on("/", handle_root);
    server.on("/rest_api", handle_rest_api);
    server.begin();
    webserver_status = main_status;
}

void WebServer::connect(void) {
    while(!connected()) {
        delay(500);
        Serial.println("Waiting to connect...");
    }
}

bool WebServer::connected(void) {
    return WiFi.status() == WL_CONNECTED;
}

void WebServer::ip_address(char *buffer) {
    if (connected()) {
        String ip = WiFi.localIP().toString();
        ip.toCharArray(buffer, 32);
    }
    else {
        sprintf(buffer, "No Wifi");
    }
}