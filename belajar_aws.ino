#include "secret.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>

WiFiClientSecure NET = WiFiClientSecure();
MQTTClient IOT = MQTTClient(512);

void cloud_incoming(String &topic, String &payload) {
  Serial.println("cloud_incoming: " + topic + " - " + payload);
  }

void setup() {
  // put your setup code here, to run once:
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(ssid,pass);
  WiFi.setHostname("CC_Sore");
  WiFi.setAutoReconnect(true);

  NET.setCACert(AWS_CERT_CA);
  NET.setCertificate(AWS_CERT_CRT);
  NET.setPrivateKey(AWS_CERT_PRIVATE);

  IOT.begin(endpoint, 8883, NET);
  IOT.onMessage(cloud_incoming);
  
  int retries = 0;
  while (!IOT.connect("Prita") && retries < 5) {
  Serial.println(".");
  delay(100);
  retries++;
  }

  if(!IOT.connected()){
    Serial.println("Timeout OI");
    return;
   }

   Serial.println("Connected");
   IOT.subscribe("/topic");
   IOT.subscribe("/sensor/+");
   IOT.subscribe("/sensor/command");
}

void loop() {
  // put your main code here, to run repeatedly:
  IOT.loop();
}
