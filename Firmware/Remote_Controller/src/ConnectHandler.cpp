#include "ConnectHandler.h"

ConnectHandler::ConnectHandler() {
    msg_init();
    _connected = true;
    _hostIp = IPAddress(192, 168, 0, 107);
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
    if (!_connected) {
        msg_send(MSG_MATCH, NULL);
    }
    _handle();
}

void ConnectHandler::_handle(void) {
    if (msg_aviliable()) {
        msg_request_t req_data = msg_receive();
        _construct_transaction_data(req_data);

        msg_reply_t res_data;
        res_data.data = req_data;
        if (req_data.msg == MSG_MATCH) {
            if (_match()) {
                res_data.resp = true;
            }
            else {
                res_data.resp = false;
            }
        }
        else {
            if (_transmit()) {
                res_data.resp = true;
            }
            else {
                res_data.resp = false;
            }
        }
        _handle_reply(res_data);
    }
}

bool ConnectHandler::_match(void) {
    Serial.println("Matching...");
    WiFiUDP receiver, sender;
    bool res = false;
    uint32_t timeout = MATCH_TIMEOUT_MS;
    package_t& p = _package.getPackage();

    receiver.begin(MATCH_PORT);
    sender.beginPacket(IPAddress(255, 255, 255, 255), MATCH_PORT);
    uint8_t tx_buf[PKG_BUF_MAX_LEN];
    _package.pack(tx_buf, sizeof(tx_buf));
    sender.write(tx_buf, BUF_SIZE(tx_buf));
    sender.endPacket();

    while (!_connected && timeout--) {
        if (receiver.parsePacket()) {
            uint8_t rx_buf[PKG_BUF_MAX_LEN];
            memset(rx_buf, 0x00, sizeof(rx_buf));
            receiver.read(rx_buf, sizeof(rx_buf));
            if (_package.parse(rx_buf, sizeof(rx_buf))) {
                res = true;
            }
        }
        delay(1);
    }

    receiver.stop();

    return res;
}

bool ConnectHandler::_transmit(void) {
    WiFiClient client;
    bool res = false;
    uint32_t timeout = TIMEOUT_MS;
    if (client.connect(_hostIp, SERVER_PORT)) {
        uint8_t tx_buf[PKG_BUF_MAX_LEN];
        _package.pack(tx_buf, sizeof(tx_buf));
        client.write(tx_buf, BUF_SIZE(tx_buf));
        Serial.println("Sent data to host");
        while (client.connected() && timeout--) {
            if (client.available()) {
                uint8_t rx_buf[PKG_BUF_MAX_LEN];
                memset(rx_buf, 0x00, sizeof(rx_buf));
                client.read(rx_buf, sizeof(rx_buf));
                Serial.println("Received host reply");
                if (_package.parse(rx_buf, sizeof(rx_buf))) {
                    res = true;
                }
                break;
            }
            delay(1);
        }
        client.stop();
    }
    return res;
}

void ConnectHandler::_construct_transaction_data(msg_request_t& msg) {
    package_t& p = _package.getPackage();

    if (msg.msg == MSG_WRITE_CONFIG) {
        Serial.println("Write config message");
        p.cmd = PKG_CMD_WRITE_SETTING;
        p.data.strip.power = configuration.power;
        if (configuration.mode == MODE_NORMAL) {
            p.data.strip.mode = PKG_MODE_NORMAL;
            p.data.strip.setting.normal.color = lv_color_to16(configuration.setting.normal.color);
        }
        else if (configuration.mode == MODE_BREATHING) {
            p.data.strip.mode = PKG_MODE_BREATHING;
            p.data.strip.setting.breathing.color = lv_color_to16(configuration.setting.breathing.color);
            p.data.strip.setting.breathing.duration = configuration.setting.breathing.duration;
            p.data.strip.setting.breathing.ease = configuration.setting.breathing.ease;
            p.data.strip.setting.breathing.interval = configuration.setting.breathing.interval;
        }
        else if (configuration.mode == MODE_LIGHTBEAM) {
            p.data.strip.mode = PKG_MODE_LIGHTBEAM;
            p.data.strip.setting.lightbeam.color = lv_color_to16(configuration.setting.lightbeam.color);
            p.data.strip.setting.lightbeam.dir = configuration.setting.lightbeam.dir;
            p.data.strip.setting.lightbeam.faded_end = configuration.setting.lightbeam.faded_end;
            p.data.strip.setting.lightbeam.head_len = configuration.setting.lightbeam.head_len;
            p.data.strip.setting.lightbeam.interval = configuration.setting.lightbeam.interval;
            p.data.strip.setting.lightbeam.len = configuration.setting.lightbeam.len;
            p.data.strip.setting.lightbeam.speed = configuration.setting.lightbeam.speed;
            p.data.strip.setting.lightbeam.tail_len = configuration.setting.lightbeam.tail_len;
        }
        else if (configuration.mode == MODE_RAINBOW) {
            p.data.strip.mode = PKG_MODE_RAINBOW;
            p.data.strip.setting.rainbow.speed = configuration.setting.rainbow.speed;
        }
    }
    else if (msg.msg == MSG_READ_CONFIG) {
        Serial.println("Read config message");
        p.cmd = PKG_CMD_READ_SETTING;
    }
    else if (msg.msg == MSG_MATCH) {
        IPAddress ip = WiFi.localIP();
        p.cmd = PKG_CMD_ACK;
        p.data.ip.a = ip[0];
        p.data.ip.b = ip[1];
        p.data.ip.c = ip[2];
        p.data.ip.d = ip[3];
    }
}

void ConnectHandler::_handle_reply(msg_reply_t& reply) {
    package_t& p = _package.getPackage();
    if (reply.data.msg == MSG_WRITE_CONFIG) {
        configuration_t* data = (configuration_t*) reply.data.user_data;
        if (!reply.resp) {
            memcpy(&configuration, data, sizeof(configuration_t));  // if it failed, roll back to the old config
        }
        heap_caps_free(data);
        data = NULL;
        lv_msg_send(RES_CONFIG_UPDATED, &reply);
    }
    else if (reply.data.msg == MSG_READ_CONFIG) {
        if (reply.resp) {
            configuration.power = p.data.strip.power;
            configuration.mode = ConnectHandler::packMode(p.data.strip.mode);
            if (p.data.strip.mode == PKG_MODE_NORMAL) {
                configuration.setting.normal.color = lv_color_hex(p.data.strip.setting.normal.color);
            }
            else if (p.data.strip.mode == PKG_MODE_BREATHING) {
                configuration.setting.breathing.color = lv_color_hex(p.data.strip.setting.breathing.color);
                configuration.setting.breathing.duration = p.data.strip.setting.breathing.duration;
                configuration.setting.breathing.ease = ConnectHandler::packEase(p.data.strip.setting.breathing.ease);
                configuration.setting.breathing.interval = p.data.strip.setting.breathing.interval;
            }
            else if (p.data.strip.mode == PKG_MODE_LIGHTBEAM) {
                configuration.setting.lightbeam.color = lv_color_hex(p.data.strip.setting.lightbeam.color);
                configuration.setting.lightbeam.dir = ConnectHandler::packDirection(p.data.strip.setting.lightbeam.dir);
                configuration.setting.lightbeam.faded_end = ConnectHandler::packFadedEnd(p.data.strip.setting.lightbeam.faded_end);
                configuration.setting.lightbeam.head_len = p.data.strip.setting.lightbeam.head_len;
                configuration.setting.lightbeam.interval = p.data.strip.setting.lightbeam.interval;
                configuration.setting.lightbeam.len = p.data.strip.setting.lightbeam.len;
                configuration.setting.lightbeam.speed = p.data.strip.setting.lightbeam.speed;
                configuration.setting.lightbeam.tail_len = p.data.strip.setting.lightbeam.tail_len;
            }
            else if (p.data.strip.mode == PKG_MODE_RAINBOW) {
                configuration.setting.rainbow.speed = p.data.strip.setting.rainbow.speed;
            }
        }
        lv_msg_send(RES_CONFIG_UPDATED, &reply);
    }
    else if (reply.data.msg == MSG_MATCH) {
        if (reply.resp) {
            _hostIp = IPAddress(p.data.ip.a, p.data.ip.b, p.data.ip.c, p.data.ip.d);
            Serial.printf("Host IP: %s\n", _hostIp.toString().c_str());
            _connected = true;
            Serial.println("Matched");
            msg_send(MSG_READ_CONFIG, NULL);
        }
        lv_msg_send(RES_MATCH, &reply);
    }
    Serial.printf("Send result: %d to GUI", reply.resp);
}

led_mode_t ConnectHandler::packMode(uint8_t mode) {
    switch (mode)
    {
        case PKG_MODE_NORMAL:
            return MODE_NORMAL;
            break;
        case PKG_MODE_BREATHING:
            return MODE_BREATHING;
            break;
        case PKG_MODE_LIGHTBEAM:
            return MODE_LIGHTBEAM;
            break;
        case PKG_MODE_RAINBOW:
            return MODE_RAINBOW;
            break;
        default:
            break;
    }
    return MODE_NORMAL;
}

ease_t ConnectHandler::packEase(uint8_t ease) {
    return EASE_LINEAR;
}

faded_end_t ConnectHandler::packFadedEnd(uint8_t faded_end) {
    if (faded_end & PKG_FADED_HEAD) {
        return FADED_HEAD;
    }
    else if (faded_end & PKG_FADED_TAIL) {
        return FADED_TAIL;
    }
    return FADED_DISABLE;
}

dir_t ConnectHandler::packDirection(uint8_t dir) {
    switch (dir)
    {
        case PKG_MOVE_RIGHT:
            return MOVE_RIGHT;
            break;
        case PKG_MOVE_LEFT:
            return MOVE_LEFT;
            break;
        default:
            break;
    }
    return MOVE_RIGHT;
}

ConnectHandler connHandler;
