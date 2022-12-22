#include <ConnectHandler.h>

void ConnectHandler::begin(void) {
    WiFi.begin(WIFI_SSID, WIFI_PWR);
    Serial.println("WiFi connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
    }
    Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
    _server.begin(SERVER_PORT);
}

void ConnectHandler::process(void) {
    
}
