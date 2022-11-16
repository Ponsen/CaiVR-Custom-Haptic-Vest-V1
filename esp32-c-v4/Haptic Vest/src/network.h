#include <WiFi.h>

typedef std::function<void(WiFiEventInfo_t, WiFiClass)> WifiConnectedCallback;
//typedef void (*WifiConnectedCallback)(WiFiEventInfo_t, WiFiClass);

extern void initWifi(WifiConnectedCallback);
extern void keepAlive();