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
void logTemps()
{
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

int ints[NUMMOTORS];
void onDriveCommand(String values)
{
    // Serial.println("driving");
    // Serial.print("values: ");
    // Serial.println(sValues);
    getValues(ints, values);
    driveMotors(ints, NUMMOTORS);
}

void wifiConnected(WiFiEventInfo_t event, WiFiClass Wifi)
{
  IPAddress gateWayIp = WiFi.gatewayIP();
  setupComms(gateWayIp, onDriveCommand);
}

/* MAIN */

// run once before loop
void setup()
{
  Serial.begin(115200);
  initWifi(wifiConnected);
  setupMotors();
  //motorTestSequence();
  // runScanner();
}

// program loop
void loop()
{
  keepAlive();
  //motorTest();
  // loopScanner();
}