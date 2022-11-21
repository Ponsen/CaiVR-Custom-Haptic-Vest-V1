#include <Arduino.h>
#include "AsyncUDP.h"
#include <ArduinoJson.h>
#include <string>
#include <WiFi.h>
#include <motor.h>
#include <stdlib.h>
#include <comm.h>

AsyncUDP udp;

int Message_Type_Register = 1;
boolean registered = false;

//interfaces
OnMessageReceived onMessageReceivedCb;

void sendMessage(char const *message)
{
    Serial.print("sending message: ");
    Serial.println(message);
    udp.print(message);
}

// TODO send available motors
void registerDevice()
{
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

void debugOut(AsyncUDPPacket packet)
{
    Serial.print("UDP Packet Type: ");
    Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast"
                                                                           : "Unicast");
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

bool prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}

void handleMessage(AsyncUDPPacket packet)
{
    String message = String(packet.data(), packet.length());

    if (message == "registered")
    {
        Serial.println("device registered");
        registered = true;
    }

    onMessageReceivedCb(message);
}

void setupComms(IPAddress gateWayIp, OnMessageReceived cb)
{
    Serial.print("UDP connecting on IP:");
    Serial.println(gateWayIp);

    onMessageReceivedCb = cb;

    if (udp.connect(gateWayIp, 1234))
    {
        Serial.println("UDP connected");
        udp.onPacket([](AsyncUDPPacket packet)
                     {
            debugOut(packet);
            //reply to the client
            //packet.printf("Got %u bytes of data", packet.length());
            handleMessage(packet); });

        int tryCount = 1;
        while (!registered)
        {
            Serial.print("registering device ");
            Serial.println(tryCount);
            registerDevice();
            delay(5000);
            tryCount++;
        }
    }
}

void sendHeartBeat()
{
    // delay(1000);
}