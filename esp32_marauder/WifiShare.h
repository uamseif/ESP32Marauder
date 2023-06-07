#ifndef WifiShare_h
#define WifiShare_h

#include "configs.h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#include "Assets.h"

#ifdef HAS_SCREEN
#include "Display.h"
#endif

#include "WiFiScan.h"

#ifdef HAS_SCREEN
extern Display display_obj;
#endif

extern WiFiScan wifi_scan_obj;

class WifiShare
{
private:

    PROGMEM const char* host = "esp32marauder";
    PROGMEM const char* ssid = "MarauderSHARE";
    PROGMEM const char* password = "c1b3rtr4cks";

    bool serving = false;

    int num_sta = 0;

    /*
     * Server Index Page
     */

    PROGMEM String server_start =
            "<h2>SPIFFS file listing /</h2>"
            "<hr>"
            "<ul>";

    PROGMEM String server_end =
            "</ul>"
            "<hr>";
    String server_index = "";


public:

    WifiShare();

    void main();
    PROGMEM static void onJavaScript();
    void setupWifiShare();
    void shutdownServer();
};

#endif
