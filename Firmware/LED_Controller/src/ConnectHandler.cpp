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
            stripHandler.setMode(MODE_NORMAL);
            normal_data_t data;
            uint32_t rgb888 = Package::RGB565toRGB888(p.data.settings.normal_setting.color);
            data.color = (RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            stripHandler.NormalFeature::setData(data);
        }
        else if (p.data.mode == PKG_MODE_BREATHING) {
            Serial.println("Mode: breathing");
            stripHandler.setMode(MODE_BREATHING);
            breathing_data_t data;
            uint32_t rgb888 = Package::RGB565toRGB888(p.data.settings.breathing_setting.color);
            data.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            data.duration = p.data.settings.breathing_setting.duration;
            data.interval = p.data.settings.breathing_setting.interval;
            data.ease = Package::translateEase(p.data.settings.breathing_setting.ease);
            stripHandler.BreathingFeature::setData(data);
        }
        else if (p.data.mode == PKG_MODE_LIGHTBEAM) {
            Serial.println("Mode: lightbeam");
            stripHandler.setMode(MODE_LIGHTBEAM);
            lightbeam_data_t data;
            uint32_t rgb888 = Package::RGB565toRGB888(p.data.settings.lightbeam_setting.color);
            data.color = RgbColor(RGB888_R(rgb888), RGB888_G(rgb888), RGB888_B(rgb888));
            data.interval = p.data.settings.lightbeam_setting.interval;
            data.len = p.data.settings.lightbeam_setting.len;
            data.speed = p.data.settings.lightbeam_setting.speed;
            data.tail_len = p.data.settings.lightbeam_setting.tail_len;
            data.head_len = p.data.settings.lightbeam_setting.head_len;
            data.faded_end = p.data.settings.lightbeam_setting.faded_end;
            data.dir = Package::translateDirection(p.data.settings.lightbeam_setting.dir);
            stripHandler.LightbeamFeature::setData(data);
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
