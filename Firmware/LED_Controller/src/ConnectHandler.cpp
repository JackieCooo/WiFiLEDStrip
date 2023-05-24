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
    Serial.printf("Matching server running at port %d\n", MATCH_PORT);
}

void ConnectHandler::process(void) {
    uint8_t rx_buf[PKG_BUF_MAX_LEN];

    // respond to client request
    WiFiClient client = _server.available();
    if (client) {
        Serial.println("Client connected");
        while (client.connected()) {
            memset(rx_buf, 0x00, sizeof(rx_buf));
            if (client.available()) {
                package_t p;

                client.read(rx_buf, sizeof(rx_buf));
                if (Package::parse(rx_buf, sizeof(rx_buf), p)) {
                    _handle(client, p);
                }
                break;
            }
        }
        client.stop();
    }

    // respond to match request
    if (_matcher.parsePacket()) {
        package_t p;

        memset(rx_buf, 0x00, sizeof(rx_buf));
        _matcher.read(rx_buf, sizeof(rx_buf));
        if (Package::parse(rx_buf, sizeof(rx_buf), p)) {
            WiFiUDP sender;
            uint8_t tx_buf[PKG_BUF_MAX_LEN];
            IPAddress clientIP(p.data.ip.addr);
            
            Serial.printf("Match request from %s\n", clientIP.toString().c_str());
            Package::accquire(p, CMD_MATCH_REPLY);
            Package::pack(tx_buf, sizeof(tx_buf), p);

            sender.beginPacket(clientIP, MATCH_PORT);
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

void ConnectHandler::_handle(WiFiClient& client, package_t& pack) {
    uint8_t tx_buf[PKG_BUF_MAX_LEN];

    if (pack.cmd == CMD_WRITE) {
        Package::apply(pack);
        stripHandler.refresh();

        local_file_t cmd = FILE_CONFIG;
        xQueueSend(saveFileMessage, &cmd, QUEUE_TIMEOUT_MS);

        Package::accquire(pack, CMD_WRITE_REPLY);
        Package::pack(tx_buf, sizeof(tx_buf), pack);
    }
    else if (pack.cmd == CMD_READ) {
        Serial.println("Read setting cmd");

        Package::accquire(pack, CMD_READ_REPLY);
        Package::pack(tx_buf, sizeof(tx_buf), pack);
    }
    else if (pack.cmd == CMD_ACK) {
        Serial.println("Ack cmd");

        Package::accquire(pack, CMD_ACK_REPLY);
        Package::pack(tx_buf, sizeof(tx_buf), pack);
    }

    client.write(tx_buf, BUF_SIZE(tx_buf));
}

ConnectHandler connHandler;
