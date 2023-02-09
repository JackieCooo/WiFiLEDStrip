#include "ConnectHandler.h"

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
    _matcher.begin(MATCH_PORT);
    Serial.printf("Match server running at port %d\n", MATCH_PORT);
}

void ConnectHandler::process(void) {
    WiFiClient client = _server.available();
    if (client) {
        Serial.println("Client connected");
        while (client.connected()) {
            uint8_t rx_buf[PKG_BUF_MAX_LEN];
            memset(rx_buf, 0x00, sizeof(rx_buf));
            if (client.available()) {
                client.read(rx_buf, sizeof(rx_buf));
                if (_package.parse(rx_buf, sizeof(rx_buf))) {
                    _handle(client);
                }
                break;
            }
        }
        client.stop();
    }

    // respond to match request
    if (_matcher.parsePacket()) {
        uint8_t rx_buf[PKG_BUF_MAX_LEN];
        memset(rx_buf, 0x00, sizeof(rx_buf));
        _matcher.read(rx_buf, sizeof(rx_buf));
        if (_package.parse(rx_buf, sizeof(rx_buf))) {
            WiFiUDP sender;
            uint8_t tx_buf[PKG_BUF_MAX_LEN];
            IPAddress ip = _package.parseTargetIP();
            Serial.printf("Match request from %s\n", ip.toString().c_str());
            _package.pack(tx_buf, PKG_CMD_MATCH_REPLY);
            sender.beginPacket(ip, MATCH_PORT);
            sender.write(tx_buf, BUF_SIZE(tx_buf));
            sender.endPacket();
        }
    }
}

void ConnectHandler::task(void* args) {
    for (;;) {
        connHandler.process();
        vTaskDelay(10);
    }
}

void ConnectHandler::_handle(WiFiClient& client) {
    package_t& p = _package.getPackage();
    uint8_t tx_buf[PKG_BUF_MAX_LEN];
    if (p.cmd == PKG_CMD_WRITE_SETTING) {
        _package.parseFromPackage();
        stripHandler.refresh();

        local_file_t cmd = FILE_CONFIG;
        xQueueSend(saveFileMessage, &cmd, QUEUE_TIMEOUT_MS);

        _package.pack(tx_buf, PKG_CMD_WRITE_REPLY);
    }
    else if (p.cmd == PKG_CMD_READ_SETTING) {
        Serial.println("Read setting cmd");

        _package.pack(tx_buf, PKG_CMD_READ_REPLY);
    }
    else if (p.cmd == PKG_CMD_ACK) {
        Serial.println("Ack cmd");

        _package.pack(tx_buf, PKG_CMD_ACK_REPLY);
    }

    client.write(tx_buf, BUF_SIZE(tx_buf));
}

ConnectHandler connHandler;
