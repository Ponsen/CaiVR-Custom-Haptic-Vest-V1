#include <Arduino.h>
#include <network.h>
#include <comm.h>
#include <WiFi.h>
#include <motor.h>
#include "AsyncUDP.h"

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

void getValues(int values[], String str)
{
    int r = 0, t = 0;

    for (int i = 0; i < str.length(); i++)
    {
        if (str.charAt(i) == ',')
        {
            values[t] = str.substring(r, i).toInt();
            r = (i + 1);
            t++;
        }
    }
}

int ints[32];
String sValues;
void OnHandleMessage(String message) {
      if (message.startsWith("drive"))
    {
        //Serial.println("driving");
        sValues = message.substring(message.indexOf("|") + 1);
        //Serial.print("values: ");
        //Serial.println(sValues);
        getValues(ints, sValues);
        driveMotors(ints, 2);
    }
}

void WifiConnected(WiFiEventInfo_t event, WiFiClass Wifi)
{
  IPAddress gateWayIp =  WiFi.gatewayIP();
  setupComms(gateWayIp, OnHandleMessage);
}



/* MAIN */

// run once before loop
void setup()
{
  Serial.begin(115200);
  initWifi(WifiConnected);
  setupMotors();
  //runScanner();
}

//program loop
void loop()
{
  keepAlive();
  //motorTest();
  //loopScanner();
}