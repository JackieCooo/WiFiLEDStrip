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

        msg_request_t request;
        request.msg = MSG_WRITE_CONFIG;
        request.user_data = NULL;
        xQueueSend(messageHandler, &request, 1000);

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
    else if (p.cmd == PKG_CMD_MATCH) {
        Serial.println("Match cmd");

        _package.pack(tx_buf, PKG_CMD_MATCH_REPLY);
    }

    client.write(tx_buf, BUF_SIZE(tx_buf));
}

ConnectHandler connHandler;
