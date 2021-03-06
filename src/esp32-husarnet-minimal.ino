#include "Arduino.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Husarnet.h>
#define HTTP_PORT 80

/* WiFi credentials */
const char* ssid = "my-wifi-network";
const char* pswd = "my-wifi-password";

/* Husarnet credentials (visit app.husarnet.com to get your JoinCode) */
const char* hostName = "esp32-webserver";
const char* JoinCode = "fc94:b01d:1803:8dd8:b293:5c7d:7639:932a/xxxxxxxxxxxxxxxxxxxxxx";

/* store index.html content in html constant variable (platformio feature) */
extern const char index_html_start[] asm("_binary_src_index_html_start");
const String html = String((const char*)index_html_start);
AsyncWebServer server(HTTP_PORT);

void setup() {
  Serial.begin(115200);

  /* Connecting to WiFi */
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  /* Initializing Husarnet VPN client*/
  Husarnet.selfHostedSetup("default");
  Husarnet.join(JoinCode, hostName);
  Husarnet.start();

  /* Start a web server */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", html);
  });
  server.begin();

  /* Print a pretty log */
  Serial.println("*******************************************");
  Serial.println("Visit your ESP32 hosted webserver:");
  Serial.println("🏠 LAN:\t\thttp://" + WiFi.localIP().toString() + ":" + String(HTTP_PORT));
  Serial.println("🌎 Internet:\thttp://" + String(hostName) + ":" + String(HTTP_PORT));
  Serial.println("*******************************************");
}

void loop() {}