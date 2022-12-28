#include "ConnectHandler.h"

ConnectHandler::ConnectHandler() {
    _connected = false;
}

void ConnectHandler::begin(void) {
    WiFi.begin(WIFI_SSID, WIFI_PWR);
    Serial.print("WiFi connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.printf("\nIP: %s\n", WiFi.localIP().toString().c_str());
}

void ConnectHandler::process(void) {
    if (_connected) {
        _handleRequest();
    }
    else {
        Serial.println("Matching strip...");
        _match();
        if (_connected) Serial.println("Matched");
        else Serial.println("Match failed");
    }
}

void ConnectHandler::_handleRequest(void) {
}

void ConnectHandler::_match(void) {
    WiFiUDP receiver, sender;
    uint32_t timeout = MATCH_MAX_TIME;
    package_t& p = _package.getPackage();

    receiver.begin(MATCH_PORT);
    sender.beginPacket(IPAddress(255, 255, 255, 255), MATCH_PORT);
    IPAddress ip = WiFi.localIP();
    p.cmd = PKG_CMD_ACK;
    p.data.ip.a = ip[0];
    p.data.ip.b = ip[1];
    p.data.ip.c = ip[2];
    p.data.ip.d = ip[3];
    uint8_t tx_buf[PKG_BUF_MAX_LEN];
    memset(tx_buf, 0x00, sizeof(tx_buf));
    _package.pack(tx_buf, sizeof(tx_buf));
    sender.write(tx_buf, BUF_SIZE(tx_buf));
    sender.endPacket();

    while (!_connected && timeout--) {
        if (receiver.parsePacket()) {
            uint8_t rx_buf[PKG_BUF_MAX_LEN];
            memset(rx_buf, 0x00, sizeof(rx_buf));
            receiver.read(rx_buf, sizeof(rx_buf));
            if (_package.parse(rx_buf, sizeof(rx_buf))) {
                _stripIP = IPAddress(p.data.ip.a, p.data.ip.b, p.data.ip.c, p.data.ip.d);
                Serial.printf("Strip IP: %s\n", _stripIP.toString().c_str());
                _connected = true;
            }
        }
        delay(1);
    }

    receiver.stop();
}

ConnectHandler connHandler;
