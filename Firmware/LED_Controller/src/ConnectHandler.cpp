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
            }
            if (_package.parse(rx_buf, sizeof(rx_buf))) {
                _handleRequest(client);
            }
        }
        client.stop();
    }
}

void ConnectHandler::_handleRequest(WiFiClient& client) {
    package_t& p = _package.getPackage();
    if (p.cmd == PKG_CMD_WRITE_SETTING) {
        Serial.println("Write setting cmd");
        configuration.power = p.data.strip.power;
        configuration.mode = Package::packMode(p.data.strip.mode);
        if (p.data.strip.mode == PKG_MODE_NORMAL) {
            Serial.println("Mode: normal");
            uint32_t rgb888 = Package::RGB565toRGB888(p.data.strip.setting.normal.color);
            configuration.setting.normal.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
        }
        else if (p.data.strip.mode == PKG_MODE_BREATHING) {
            Serial.println("Mode: breathing");
            uint32_t rgb888 = Package::RGB565toRGB888(p.data.strip.setting.breathing.color);
            configuration.setting.breathing.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            configuration.setting.breathing.duration = p.data.strip.setting.breathing.duration;
            configuration.setting.breathing.interval = p.data.strip.setting.breathing.interval;
            configuration.setting.breathing.ease = Package::packEase(p.data.strip.setting.breathing.ease);
        }
        else if (p.data.strip.mode == PKG_MODE_LIGHTBEAM) {
            Serial.println("Mode: lightbeam");
            uint32_t rgb888 = Package::RGB565toRGB888(p.data.strip.setting.lightbeam.color);
            configuration.setting.lightbeam.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            configuration.setting.lightbeam.gap = p.data.strip.setting.lightbeam.gap;
            configuration.setting.lightbeam.len = p.data.strip.setting.lightbeam.len;
            configuration.setting.lightbeam.speed = p.data.strip.setting.lightbeam.speed;
            configuration.setting.lightbeam.tail_len = p.data.strip.setting.lightbeam.tail_len;
            configuration.setting.lightbeam.head_len = p.data.strip.setting.lightbeam.head_len;
            configuration.setting.lightbeam.faded_end = Package::packFadedEnd(p.data.strip.setting.lightbeam.faded_end);
            configuration.setting.lightbeam.dir = Package::packDirection(p.data.strip.setting.lightbeam.dir);
        }
        else if (p.data.strip.mode == PKG_MODE_RAINBOW) {
            Serial.println("Mode: rainbow");
            configuration.setting.rainbow.speed = p.data.strip.setting.rainbow.speed;
        }
        stripHandler.refresh();
        msg_send(MSG_WRITE_CONFIG, NULL);

        p.cmd = PKG_CMD_WRITE_REPLY;
        p.data.resp.resp = true;
    }
    else if (p.cmd == PKG_CMD_READ_SETTING) {
        Serial.println("Read setting cmd");
        p.data.strip.power = configuration.power;
        strip_mode_t mode = configuration.mode;
        if (mode == MODE_NORMAL) {
            p.data.strip.mode = PKG_MODE_NORMAL;
            RgbColor color = configuration.setting.normal.color;
            uint32_t rgb888 = CONCAT_RGB888(color.R, color.G, color.B);
            p.data.strip.setting.normal.color = Package::RGB888toRGB565(rgb888);
        }
        else if (mode == MODE_BREATHING) {
            p.data.strip.mode = PKG_MODE_BREATHING;
            RgbColor color = configuration.setting.breathing.color;
            uint32_t rgb888 = CONCAT_RGB888(color.R, color.G, color.B);
            p.data.strip.setting.breathing.color = Package::RGB888toRGB565(rgb888);
            p.data.strip.setting.breathing.duration = configuration.setting.breathing.duration;
            p.data.strip.setting.breathing.interval = configuration.setting.breathing.interval;
            p.data.strip.setting.breathing.ease = Package::parseEase(configuration.setting.breathing.ease);
        }
        else if (mode == MODE_LIGHTBEAM) {
            p.data.strip.mode = PKG_MODE_LIGHTBEAM;
            RgbColor color = configuration.setting.lightbeam.color;
            uint32_t rgb888 = CONCAT_RGB888(color.R, color.G, color.B);
            p.data.strip.setting.lightbeam.color = Package::RGB888toRGB565(rgb888);
            p.data.strip.setting.lightbeam.len = configuration.setting.lightbeam.len;
            p.data.strip.setting.lightbeam.gap = configuration.setting.lightbeam.gap;
            p.data.strip.setting.lightbeam.speed = configuration.setting.lightbeam.speed;
            p.data.strip.setting.lightbeam.faded_end = Package::parseFadedEnd(configuration.setting.lightbeam.faded_end);
            p.data.strip.setting.lightbeam.head_len = configuration.setting.lightbeam.head_len;
            p.data.strip.setting.lightbeam.tail_len = configuration.setting.lightbeam.tail_len;
            p.data.strip.setting.lightbeam.dir = configuration.setting.lightbeam.dir;
        }
        else if (mode == MODE_RAINBOW) {
            p.data.strip.mode = PKG_MODE_RAINBOW;
            p.data.strip.setting.rainbow.speed = configuration.setting.rainbow.speed;
        }
        p.cmd = PKG_CMD_READ_REPLY;
    }
    else if (p.cmd == PKG_CMD_ACK) {
        Serial.println("Ack cmd");
        p.cmd = PKG_CMD_ACK_REPLY;
    }

    uint8_t tx_buf[PKG_BUF_MAX_LEN];
    _package.pack(tx_buf, sizeof(tx_buf));
    client.write(tx_buf, BUF_SIZE(tx_buf));
}

ConnectHandler connHandler;
