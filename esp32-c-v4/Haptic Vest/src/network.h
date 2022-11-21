#include <WiFi.h>

typedef std::function<void(WiFiEventInfo_t, WiFiClass)> WifiConnectedCallback;

extern void initWifi(WifiConnectedCallback);
extern void keepAlive();