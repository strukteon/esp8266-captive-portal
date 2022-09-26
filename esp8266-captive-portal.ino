#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGratuitous.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <DNSServer.h>

#include <FS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define FORMAT_SPIFFS_IF_FAILED true

const byte DNS_PORT = 53;
IPAddress apIP(8,8,4,4); // The default android DNS
DNSServer dnsServer;
AsyncWebServer webServer(80);


void setup() { 
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("ESP8266 Captive Portal");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  webServer.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(SPIFFS, "/index.html", "text/html");
    }
  });

  webServer.on("/instagram.svg", [](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(SPIFFS, "/instagram.svg", "image/svg+xml");
    }
  });


  webServer.on("/snapchat.svg", [](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(SPIFFS, "/snapchat.svg", "image/svg+xml");
    }
  });

  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
}
