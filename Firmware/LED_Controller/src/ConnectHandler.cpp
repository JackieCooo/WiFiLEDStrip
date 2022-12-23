#include <ConnectHandler.h>

void ConnectHandler::begin(void) {
    WiFi.begin(WIFI_SSID, WIFI_PWR);
    Serial.print("WiFi connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.printf("\nIP: %s\n", WiFi.localIP().toString().c_str());
    _server.begin(SERVER_PORT);
    Serial.printf("Server running at port %d\n", SERVER_PORT);
}

void ConnectHandler::process(void) {
    WiFiClient client = _server.available();
    if (client) {
        Serial.println("Client connected");
        while (client.connected()) {
            uint8_t rx_buf[64];
            memset(rx_buf, 0x00, sizeof(rx_buf));
            if (client.available()) {
                client.readBytes(rx_buf, sizeof(rx_buf));
            }
            if (_package.parse(rx_buf, sizeof(rx_buf))) {
                _handleRequest();
            }
        }
        client.stop();
    }
}

void ConnectHandler::_handleRequest(void) {
    package_t p = _package.getPackage();
    if (p.cmd == PKG_CMD_WRITE_SETTING) {
        Serial.println("Write setting cmd");
        if (p.data.mode == PKG_MODE_NORMAL) {
            Serial.println("Mode: normal");
            
        }
        else if (p.data.mode == PKG_MODE_BREATHING) {
            Serial.println("Mode: breathing");
        }
        else if (p.data.mode == PKG_MODE_LIGHTBEAM) {
            Serial.println("Mode: lightbeam");
        }
        else if (p.data.mode == PKG_MODE_RAINBOW) {
            Serial.println("Mode: rainbow");
        }
    }
    else if (p.cmd == PKG_CMD_READ_SETTING) {
        Serial.println("Read setting cmd");
    }
    else if (p.cmd == PKG_CMD_ACK) {
        Serial.println("Ack cmd");
    }
}

ConnectHandler connHandler;
