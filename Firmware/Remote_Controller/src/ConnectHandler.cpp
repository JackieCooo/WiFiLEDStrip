#include "ConnectHandler.h"

ConnectHandler::ConnectHandler() {
    _connected = true;
}

void ConnectHandler::begin(void) {
    WiFi.begin(WIFI_SSID, WIFI_PWR);
    Serial.print("WiFi connecting");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.printf("\nIP: %s\n", WiFi.localIP().toString().c_str());
}

void ConnectHandler::process(void) {
    if (_connected) {
        _handle();
    }
    else {
        Serial.println("Matching strip...");
        _match();
        if (_connected) Serial.println("Matched");
        else Serial.println("Match failed");
    }
}

void ConnectHandler::_handle(void) {
    if (!queue_empty()) {
        msg_struct_t data = queue_front();
        package_t& p = _package.getPackage();

        if (data.msg == MSG_MODE_CHANGED) {
            Serial.println("Mode changed message");
            p.data.strip.subcmd = PKG_DATA_ALL;
            if (data.mode == PKG_MODE_NORMAL) {
                p.data.strip.mode = PKG_MODE_NORMAL;
                p.data.strip.setting.normal.color = data.setting.normal.color;
            }
            else if (data.mode == PKG_MODE_BREATHING) {
                p.data.strip.mode = PKG_MODE_BREATHING;
                p.data.strip.setting.breathing.color = data.setting.breathing.color;
                p.data.strip.setting.breathing.duration = data.setting.breathing.duration;
                p.data.strip.setting.breathing.ease = data.setting.breathing.ease;
                p.data.strip.setting.breathing.interval = data.setting.breathing.interval;
            }
            else if (data.mode == PKG_MODE_LIGHTBEAM) {
                p.data.strip.mode = PKG_MODE_LIGHTBEAM;
                p.data.strip.setting.lightbeam.color = data.setting.lightbeam.color;
                p.data.strip.setting.lightbeam.dir = data.setting.lightbeam.dir;
                p.data.strip.setting.lightbeam.faded_end = data.setting.lightbeam.faded_end;
                p.data.strip.setting.lightbeam.head_len = data.setting.lightbeam.head_len;
                p.data.strip.setting.lightbeam.interval = data.setting.lightbeam.interval;
                p.data.strip.setting.lightbeam.len = data.setting.lightbeam.len;
                p.data.strip.setting.lightbeam.speed = data.setting.lightbeam.speed;
                p.data.strip.setting.lightbeam.tail_len = data.setting.lightbeam.tail_len;
            }
            else if (data.mode == PKG_MODE_RAINBOW) {
                p.data.strip.mode = PKG_MODE_RAINBOW;
                p.data.strip.setting.rainbow.speed = data.setting.rainbow.speed;
            }
        }
        else if (data.msg == MSG_COLOR_CHANGED) {
            Serial.println("Color changed message");
            if (data.mode == PKG_MODE_NORMAL) {
                p.data.strip.mode = PKG_MODE_NORMAL;
                p.data.strip.setting.normal.color = data.setting.normal.color;
            }
            else if (data.mode == PKG_MODE_BREATHING) {
                p.data.strip.mode = PKG_MODE_BREATHING;
                p.data.strip.setting.breathing.color = data.setting.breathing.color;
            }
            else if (data.mode == PKG_MODE_LIGHTBEAM) {
                p.data.strip.mode = PKG_MODE_LIGHTBEAM;
                p.data.strip.setting.lightbeam.color = data.setting.lightbeam.color;
            }
        }

        uint8_t tx_buf[PKG_BUF_MAX_LEN];
        _package.pack(tx_buf, sizeof(tx_buf));
        _transmit(tx_buf, BUF_SIZE(tx_buf));
        msg_reply_t reply;
        reply.msg = data.msg;
        reply.resp = p.data.resp.resp;
        lv_msg_send(MSG_REPLY, &reply);

        queue_pop();
    }
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
                _hostIp = IPAddress(p.data.ip.a, p.data.ip.b, p.data.ip.c, p.data.ip.d);
                Serial.printf("Strip IP: %s\n", _hostIp.toString().c_str());
                _connected = true;
            }
        }
        delay(1);
    }

    receiver.stop();
}

bool ConnectHandler::_transmit(uint8_t* buf, uint8_t size) {
    WiFiClient client;
    bool res = false;
    uint32_t timeout = TIMEOUT_TIME;
    if (client.connect(_hostIp, SERVER_PORT)) {
        client.write(buf, size);
        Serial.println("Sent data to host");
        while (client.connected() && timeout--) {
            if (client.available()) {
                uint8_t rx_buf[PKG_BUF_MAX_LEN];
                memset(rx_buf, 0x00, sizeof(rx_buf));
                client.read(rx_buf, sizeof(rx_buf));
                Serial.println("Received host reply");
                if (_package.parse(rx_buf, sizeof(rx_buf))) {
                    package_t& p = _package.getPackage();
                    if (p.cmd == PKG_CMD_WRITE_REPLY && p.data.resp.resp == PKG_REPLY_OK) {
                        res = true;
                        break;
                    }
                }
            }
            delay(1);
        }
        client.stop();
    }
    return res;
}

ConnectHandler connHandler;
