#include "ConfigHandler.h"

configuration_t configuration;
xQueueHandle saveConfigMessage;

ConfigHandler::ConfigHandler() {
    saveConfigMessage = xSemaphoreCreateBinary();
}

void ConfigHandler::begin(void) {
    // SPIFFS.format();
    if (SPIFFS.begin(true)) {
        Serial.println("SPIFFS init done");
    }
    else {
        Serial.println("SPIFFS init failed");
        while(true);
    }
}

void ConfigHandler::load(void) {
    File file = SPIFFS.open(CONFIG_FILE_PATH, FILE_READ);
    if (file) {
        Serial.println("Loading configuration");
        uint8_t buf[sizeof(configuration_t)];
        memset(buf, 0x00, sizeof(buf));
        file.read(buf, sizeof(configuration_t));
        memcpy(&configuration, buf, sizeof(configuration_t));
        file.close();
    }
    else {
        Serial.println("File open failed");
    }
}

void ConfigHandler::save(void) {
    File file = SPIFFS.open(CONFIG_FILE_PATH, FILE_WRITE);
    if (file) {
        Serial.println("Saving configuration");
        file.write((uint8_t*)&configuration, sizeof(configuration_t));
        file.close();
    }
    else {
        Serial.println("File open failed");
    }
}

void ConfigHandler::process(void) {
    if (xSemaphoreTake(saveConfigMessage, portMAX_DELAY)) {
        save();
    }
}

void ConfigHandler::task(void* args) {
    for (;;) {
        configHandler.process();
        vTaskDelay(10);
    }
}

void ConfigHandler::_initConfigurationSetting(File& file) {
    configuration_t config;
    config.mode = MODE_NORMAL;
    config.power = false;
    config.setting.normal.color = lv_palette_main(LV_PALETTE_RED);
    config.setting.breathing.color = lv_palette_main(LV_PALETTE_RED);
    config.setting.breathing.duration = 2000;
    config.setting.breathing.ease = EASE_LINEAR;
    config.setting.breathing.interval = 1000;
    config.setting.lightbeam.color = lv_palette_main(LV_PALETTE_RED);
    config.setting.lightbeam.dir = MOVE_LEFT;
    faded_end_t faded_end = {0};
    config.setting.lightbeam.faded_end = faded_end;
    config.setting.lightbeam.gap = 2;
    config.setting.lightbeam.head_len = 4;
    config.setting.lightbeam.len = 4;
    config.setting.lightbeam.speed = 8;
    config.setting.lightbeam.tail_len = 4;
    config.setting.rainbow.speed = 8;

    file.write((uint8_t*)&config, sizeof(configuration_t));
}

void ConfigHandler::_initConnectivitySetting(File& file) {
    connectivity_t connect;
    connect.connected = false;
    ip_addr_t host_ip = {0};
    connect.host_ip = host_ip;
    connect.matched = false;
    connect.password = NULL;
    connect.ssid = NULL;

    file.write((uint8_t*)&connect, sizeof(connectivity_t));
}

void ConfigHandler::_checkLocalFiles(void) {
    if (!SPIFFS.exists(CONFIG_FILE_PATH)) {
        Serial.println("Config file not find, creating");
        File file = SPIFFS.open(CONFIG_FILE_PATH, FILE_WRITE, true);
        _initConfigurationSetting(file);
        file.close();
        Serial.println("Config file created");
    }
    if (!SPIFFS.exists(WIFI_CONFIG_FILE_PATH)) {
        Serial.println("WiFi config file not find, creating");
        File file = SPIFFS.open(WIFI_CONFIG_FILE_PATH, FILE_WRITE, true);
        _initConnectivitySetting(file);
        file.close();
        Serial.println("WiFi config file created");
    }
}

ConfigHandler configHandler;
