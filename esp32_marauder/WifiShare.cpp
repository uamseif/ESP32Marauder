#include "WifiShare.h"
#include "lang_var.h"

WebServer server(80);

WifiShare::WifiShare()
{

}

void WifiShare::main()
{
  // Notify if client has connected to the update server


  int current_sta = WiFi.softAPgetStationNum();

  if (current_sta < this->num_sta)
  {
    this->num_sta = current_sta;
    Serial.print("Update server: Client disconnected -> ");
    Serial.println(this->num_sta);
  }
  else if (current_sta > this->num_sta)
  {
    this->num_sta = current_sta;
    Serial.print("Update server: Client connected -> ");
    Serial.println(this->num_sta);
  }


  server.handleClient();
  delay(1);
}

// Callback for the embedded jquery.min.js page
void WifiShare::onJavaScript(void) {
  Serial.println("onJavaScript(void)");
  server.setContentLength(jquery_min_js_v3_2_1_gz_len);
  server.sendHeader(F("Content-Encoding"), F("gzip"));
  server.send_P(200, "text/javascript", jquery_min_js_v3_2_1_gz, jquery_min_js_v3_2_1_gz_len);
}

void WifiShare::setupWifiShare()
{
  uint8_t newMACAddress[] = {0x06, 0x07, 0x0D, 0x09, 0x0E, 0x0D};

#ifdef HAS_SCREEN
  display_obj.tft.setTextWrap(false);
  display_obj.tft.setFreeFont(NULL);
  display_obj.tft.setCursor(0, 100);
  display_obj.tft.setTextSize(1);
  display_obj.tft.setTextColor(TFT_WHITE);
#endif

  Serial.println(wifi_scan_obj.freeRAM());
#ifdef HAS_SCREEN
  display_obj.tft.print(text_table3[0]);
#endif
  Serial.println("Configuring update server...");

#ifdef HAS_SCREEN
  display_obj.tft.setTextColor(TFT_YELLOW);
#endif

  // Start WiFi AP
  Serial.println("Initializing WiFi...");
  //wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&wifi_scan_obj.cfg);
  //esp_wifi_set_storage(WIFI_STORAGE_RAM);
  if (esp_wifi_set_storage(WIFI_STORAGE_FLASH) != ESP_OK)
    Serial.println("Could not set WiFi Storage!");
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();
  Serial.println(wifi_scan_obj.freeRAM());

  Serial.println("Starting softAP...");
  esp_wifi_set_mac(WIFI_IF_AP, &newMACAddress[0]);
  WiFi.softAP(ssid, password);
  Serial.println("");

  Serial.println(wifi_scan_obj.freeRAM());

  Serial.println("Displaying settings to TFT...");
#ifdef HAS_SCREEN
  display_obj.tft.print(text_table1[2]);
  display_obj.tft.println(ssid);
  display_obj.tft.print(text_table3[1]);
  display_obj.tft.print(WiFi.softAPIP());
  display_obj.tft.print("\n");
#endif
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // return javascript jquery
  Serial.println("Setting server behavior...");
  Serial.println(wifi_scan_obj.freeRAM());
  server.on("/jquery.min.js", HTTP_GET, onJavaScript);

  server_index += server_start;

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while(file){
    server_index += "<li><a href=\"/download?file=" + String(file.name()) + "\">" + String(file.name()) + "</a></li>";
    file = root.openNextFile();
  }
  server_index += server_end;

  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, [this]() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", server_index);

  });

  server.on("/download", HTTP_GET, [this]() {
      if (server.args() > 0 && server.hasArg("file")) {
        String file = server.arg("file");
        server.sendHeader("Connection", "close");
        //server.send(SPIFFS, "/raw_0.pcap", "text/plain", true);
        File download = SPIFFS.open("/" + file,  "r");
        if (download) {
          server.sendHeader("Content-Type", "text/text");
          server.sendHeader("Content-Disposition", "attachment; filename=" + file);
          server.sendHeader("Connection", "close");
          server.streamFile(download, "application/octet-stream");
          download.close();
        }
      } else {
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", "");
        server.sendContent(server_index);
      }

  });

  Serial.println("Finished setting server behavior");
  Serial.println(wifi_scan_obj.freeRAM());
  Serial.println("Starting server...");
  server.begin();

#ifdef HAS_SCREEN
  display_obj.tft.setTextColor(TFT_GREEN);
  display_obj.tft.println(text_table3[5]);
#endif
  Serial.println("Completed update server setup");
  Serial.println(wifi_scan_obj.freeRAM());
}

void WifiShare::shutdownServer() {
  Serial.println("Closing Update Server...");
  server.stop();
  WiFi.mode(WIFI_OFF);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_stop();
  esp_wifi_deinit();
  Serial.println(wifi_scan_obj.freeRAM());
}
