#include "ConfigHandler.h"

connectivity_t connectivity;

void ConfigHandler::begin(void) {
    // SPIFFS.format();
    _saveFileMessage = xQueueCreate(1, sizeof(local_file_t));
    if (SPIFFS.begin(true)) {
        Serial.println("SPIFFS init done");
        _checkLocalFiles();
    }
    else {
        Serial.println("SPIFFS init failed");
        while(true);
    }
}

void ConfigHandler::load(void) {
    File file = SPIFFS.open(CONFIG_FILE_PATH, FILE_READ);
    if (file.available()) {
        Serial.println("Loading configuration file");
        file.read((uint8_t*)&configuration, sizeof(configuration_t));
        file.close();
    }

    file = SPIFFS.open(CONNECT_FILE_PATH, FILE_READ);
    if (file.available()) {
        Serial.println("Loading connectivity file");
        file.read((uint8_t*)&connectivity, sizeof(connectivity_t));
        file.close();
    }
}

void ConfigHandler::save(local_file_t cmd) {
    File file;
    if (cmd == FILE_CONFIG) {
        file = SPIFFS.open(CONFIG_FILE_PATH, FILE_WRITE);
    }
    else if (cmd == FILE_CONNECT) {
        file = SPIFFS.open(CONNECT_FILE_PATH, FILE_WRITE);
    }
    if (file.available()) {
        if (cmd == FILE_CONFIG) {
            Serial.println("Save configuration file");
            file.write((uint8_t*)&configuration, sizeof(configuration_t));
        }
        else if (cmd == FILE_CONNECT) {
            Serial.println("Save connectivity file");
            file.write((uint8_t*)&connectivity, sizeof(connectivity_t));
        }
        file.close();
    }
}

void ConfigHandler::process(void) {
    local_file_t cmd;
    if (xQueueReceive(_saveFileMessage, &cmd, portMAX_DELAY)) {
        save(cmd);
    }
}

void ConfigHandler::task(void* args) {
    for (;;) {
        configHandler.process();
        vTaskDelay(10);
    }
}

void ConfigHandler::saveFile(local_file_t cmd) {
    xQueueSend(_saveFileMessage, &cmd, QUEUE_TIMEOUT_MS);
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
    config.setting.lightbeam.faded_end.FADED_HEAD = 0;
    config.setting.lightbeam.faded_end.FADED_TAIL = 0;
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
    connect.host_ip.addr = 0;
    memset(connect.ssid, 0x00, sizeof(connect.ssid));
    memset(connect.password, 0x00, sizeof(connect.password));

    file.write((uint8_t*)&connect, sizeof(connectivity_t));
}

void ConfigHandler::_checkLocalFiles(void) {
    if (!SPIFFS.exists(CONFIG_FILE_PATH)) {
        Serial.println("Missing configuration file, creating...");
        File file = SPIFFS.open(CONFIG_FILE_PATH, FILE_WRITE, true);
        _initConfigurationSetting(file);
        file.close();
        Serial.println("Configuration file created");
    }
    if (!SPIFFS.exists(CONNECT_FILE_PATH)) {
        Serial.println("Missing connectivity file, creating...");
        File file = SPIFFS.open(CONNECT_FILE_PATH, FILE_WRITE, true);
        _initConnectivitySetting(file);
        file.close();
        Serial.println("connectivity file created");
    }
}

ConfigHandler configHandler;
