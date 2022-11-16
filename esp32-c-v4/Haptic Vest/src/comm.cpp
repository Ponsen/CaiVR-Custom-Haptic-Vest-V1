#include <Arduino.h>
#include "AsyncUDP.h"
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFi.h>

AsyncUDP udp;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int Message_Type_Register = 1;

void sendMessage(char const *message)
{
    Serial.print("sending message: ");
    Serial.println(message);
    udp.print(message);
}

void registerDevice () {
    std::string str = std::to_string(ESP.getEfuseMac());
    const char *mac = str.c_str();
    const char *type = "haptic-vest-v1";

    DynamicJsonDocument result(256);

    result["mtype"] = Message_Type_Register;
    result["mac"] = mac;
    result["type"] = type;
    JsonArray data = result.createNestedArray("data");
    data.add(48.756080);
    data.add(2.302038);

    char jsonChar[256];
    serializeJson(result, jsonChar, sizeof jsonChar);
    sendMessage(jsonChar);
}

void debugOut(AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
}

void setupComms(IPAddress gateWayIp)
{
    Serial.print("UDP connecting on IP:");
    Serial.println(gateWayIp);

    if(udp.connect(gateWayIp, 1234)) {
        Serial.println("UDP connected");
        udp.onPacket([](AsyncUDPPacket packet) {
            debugOut(packet);
            //reply to the client
            //packet.printf("Got %u bytes of data", packet.length());
        });
        registerDevice();
    }
    
    timeClient.begin();  
    
    // Set offset time in seconds to adjust for your timezone, for example:
    // GMT +1 = 3600
    // GMT +8 = 28800
    // GMT -1 = -3600
    // GMT 0 = 0
    timeClient.setTimeOffset(3600);
}

void sendHeartBeat() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  Serial.println(timeClient.getFormattedTime());

  sendMessage((const char*)timeClient.getEpochTime());

  delay(1000);
}