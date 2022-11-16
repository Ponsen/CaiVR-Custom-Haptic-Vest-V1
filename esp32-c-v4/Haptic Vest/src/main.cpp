#include <Arduino.h>
#include <network.h>
#include <comm.h>
#include <WiFi.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

uint8_t temprature_sens_read();

/* TEMPS */
// doesn't work if wifi is off and then it's values are not reliable
void logTemps() {
  Serial.print((temprature_sens_read() - 32) / 1.8);
  Serial.println(" C");
}

/* BROKER */

void WifiConnected(WiFiEventInfo_t event, WiFiClass Wifi)
{
  IPAddress gateWayIp =  WiFi.gatewayIP();
  setupComms(gateWayIp);
}

/* MAIN */

// run once before loop
void setup()
{
  Serial.begin(115200);
  initWifi(WifiConnected);
}

//program loop
void loop()
{
  keepAlive();
  sendHeartBeat();
}