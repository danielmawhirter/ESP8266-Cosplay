#ifndef USE_AP
  #ifndef USE_STA
    #define USE_STA
  #endif
#endif
#ifdef USE_STA
  #ifdef USE_AP
    #undef USE_AP
  #endif
#endif

#ifdef USE_STA
  //const char* ssids[] = {"skynet", "mawhirter"};
  const char* ssid = "skynet";
  const char* password = "";
  void beginWiFi() {
    //int n = WiFi.scanNetworks();
    //for(int i = 0; i < n; i++) {
  //    DBG_OUTPUT_PORT.println(sizeof(ssids)/sizeof(ssids[0]));
      //DBG_OUTPUT_PORT.print(',');
      //DBG_OUTPUT_PORT.print(sizeof(ssids[0]));
    //}
    
    //WIFI INIT
    DBG_OUTPUT_PORT.printf("Connecting to %s\n", ssid);
    if (String(WiFi.SSID()) != String(ssid)) {
      WiFi.begin(ssid, password);
    }
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      DBG_OUTPUT_PORT.print(".");
    }
    DBG_OUTPUT_PORT.println("");
    DBG_OUTPUT_PORT.print("Connected! IP address: ");
    DBG_OUTPUT_PORT.println(WiFi.localIP());
  }
#endif

#ifdef USE_AP
  const char* ssid = "samus";
  const char* password = "";
  IPAddress apIP(192, 168, 1, 1);
  void beginWiFi() {
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssid, password);
  }
#endif

