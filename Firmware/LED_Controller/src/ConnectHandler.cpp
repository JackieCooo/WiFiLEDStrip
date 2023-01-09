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
        stripHandler.setPower(p.data.strip.power);
        if (p.data.strip.mode == PKG_MODE_NORMAL) {
            Serial.println("Mode: normal");
            stripHandler.setMode(MODE_NORMAL);
            normal_data_t data;
            uint32_t rgb888 = Package::RGB565toRGB888(p.data.strip.setting.normal.color);
            data.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            stripHandler.NormalFeature::setData(data);
            Serial.printf("color: #%x\n", data.color);
        }
        else if (p.data.strip.mode == PKG_MODE_BREATHING) {
            Serial.println("Mode: breathing");
            stripHandler.setMode(MODE_BREATHING);
            breathing_data_t data;
            uint32_t rgb888 = Package::RGB565toRGB888(p.data.strip.setting.breathing.color);
            data.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            data.duration = p.data.strip.setting.breathing.duration;
            data.interval = p.data.strip.setting.breathing.interval;
            data.ease = Package::packEase(p.data.strip.setting.breathing.ease);
            stripHandler.BreathingFeature::setData(data);
            Serial.printf("color: #%x, duration: %d, interval: %d, ease: %d\n", data.color, data.duration, data.interval, data.ease);
        }
        else if (p.data.strip.mode == PKG_MODE_LIGHTBEAM) {
            Serial.println("Mode: lightbeam");
            stripHandler.setMode(MODE_LIGHTBEAM);
            lightbeam_data_t data;
            uint32_t rgb888 = Package::RGB565toRGB888(p.data.strip.setting.lightbeam.color);
            data.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            data.gap = p.data.strip.setting.lightbeam.gap;
            data.len = p.data.strip.setting.lightbeam.len;
            data.speed = p.data.strip.setting.lightbeam.speed;
            data.tail_len = p.data.strip.setting.lightbeam.tail_len;
            data.head_len = p.data.strip.setting.lightbeam.head_len;
            data.faded_end = p.data.strip.setting.lightbeam.faded_end;
            data.dir = Package::packDirection(p.data.strip.setting.lightbeam.dir);
            stripHandler.LightbeamFeature::setData(data);
            Serial.printf("color: #%x, interval: %d, len: %d, speed: %d, tail_len: %d, head_len: %d, faded_end: %d, dir: %d\n", data.color, data.gap, data.len, data.speed, data.tail_len, data.head_len, data.faded_end, data.dir);
        }
        else if (p.data.strip.mode == PKG_MODE_RAINBOW) {
            Serial.println("Mode: rainbow");
            stripHandler.setMode(MODE_RAINBOW);
            rainbow_data_t data;
            data.speed = p.data.strip.setting.rainbow.speed;
            stripHandler.RainbowFeature::setData(data);
            Serial.printf("speed: %d\n", data.speed);
        }
        p.cmd = PKG_CMD_WRITE_REPLY;
        p.data.resp.resp = true;
    }
    else if (p.cmd == PKG_CMD_READ_SETTING) {
        Serial.println("Read setting cmd");

        p.data.strip.power = stripHandler.getPower();
        strip_mode_t mode = stripHandler.getMode();
        if (mode == MODE_NORMAL) {
            p.data.strip.mode = PKG_MODE_NORMAL;
            normal_data_t data = stripHandler.NormalFeature::getData();
            RgbColor color = data.color;
            uint32_t rgb888 = CONCAT_RGB888(color.R, color.G, color.B);
            p.data.strip.setting.normal.color = Package::RGB888toRGB565(rgb888);
        }
        else if (mode == MODE_BREATHING) {
            p.data.strip.mode = PKG_MODE_BREATHING;
            breathing_data_t data = stripHandler.BreathingFeature::getData();
            RgbColor color = data.color;
            uint32_t rgb888 = CONCAT_RGB888(color.R, color.G, color.B);
            p.data.strip.setting.breathing.color = Package::RGB888toRGB565(rgb888);
            p.data.strip.setting.breathing.duration = data.duration;
            p.data.strip.setting.breathing.interval = data.interval;
            p.data.strip.setting.breathing.ease = Package::parseEase(data.ease);
        }
        else if (mode == MODE_LIGHTBEAM) {
            p.data.strip.mode = PKG_MODE_LIGHTBEAM;
            lightbeam_data_t data = stripHandler.LightbeamFeature::getData();
            RgbColor color = data.color;
            uint32_t rgb888 = CONCAT_RGB888(color.R, color.G, color.B);
            p.data.strip.setting.lightbeam.color = Package::RGB888toRGB565(rgb888);
            p.data.strip.setting.lightbeam.len = data.len;
            p.data.strip.setting.lightbeam.gap = data.gap;
            p.data.strip.setting.lightbeam.speed = data.speed;
            p.data.strip.setting.lightbeam.faded_end = data.faded_end;
            p.data.strip.setting.lightbeam.head_len = data.head_len;
            p.data.strip.setting.lightbeam.tail_len = data.tail_len;
            p.data.strip.setting.lightbeam.dir = data.dir;
        }
        else if (mode == MODE_RAINBOW) {
            p.data.strip.mode = PKG_MODE_RAINBOW;
            rainbow_data_t data = stripHandler.RainbowFeature::getData();
            p.data.strip.setting.rainbow.speed = data.speed;
        }
        p.cmd = PKG_CMD_READ_REPLY;
    }
    else if (p.cmd == PKG_CMD_ACK) {
        Serial.println("Ack cmd");
        p.cmd = PKG_CMD_ACK_REPLY;
    }

    uint8_t tx_buf[PKG_BUF_MAX_LEN];
    _package.pack(tx_buf, BUF_SIZE(tx_buf));
    client.write(tx_buf, BUF_SIZE(tx_buf));
}

ConnectHandler connHandler;
