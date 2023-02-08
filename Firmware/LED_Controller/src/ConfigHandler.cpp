#include "ConfigHandler.h"

configuration_t configuration;
connectivity_t connectivity;
xQueueHandle saveFileMessage;

void ConfigHandler::begin(void) {
    saveFileMessage = xQueueCreate(1, sizeof(local_file_t));
    // SPIFFS.format();
    if (SPIFFS.begin(true)) {
        Serial.println("SPIFFS init done");
    }
    else {
        Serial.println("SPIFFS init failed");
    }
}

void ConfigHandler::load(void) {
    _checkFileIntergrality();

    File file;

    file = SPIFFS.open(CONFIG_FILE_PATH, FILE_READ);
    if (file) {
        Serial.println("Loading configuration file");
        uint8_t buf[sizeof(configuration_t)];
        file.read(buf, sizeof(configuration_t));
        memcpy(&configuration, buf, sizeof(configuration_t));
        file.close();
    }
    file = SPIFFS.open(CONNECT_FILE_PATH, FILE_READ);
    if (file) {
        Serial.println("Loading connectivity file");
        uint8_t buf[sizeof(connectivity_t)];
        file.read(buf, sizeof(connectivity_t));
        memcpy(&connectivity, buf, sizeof(connectivity_t));
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
    if (file) {
        Serial.println("Saving...");
        if (cmd == FILE_CONFIG) {
            file.write((uint8_t*)&configuration, sizeof(configuration_t));
        }
        else if (cmd == FILE_CONNECT) {
            file.write((uint8_t*)&configuration, sizeof(configuration_t));
        }
        file.close();
    }
}

void ConfigHandler::process(void) {
    local_file_t cmd;
    if (xQueueReceive(messageHandler, &cmd, portMAX_DELAY)) {
        save(cmd);
    }
}

void ConfigHandler::task(void* args) {
    for (;;) {
        configHandler.process();
        vTaskDelay(10);
    }
}

void ConfigHandler::_checkFileIntergrality(void) {
    if (!SPIFFS.exists(CONFIG_FILE_PATH)) {
        Serial.println("Missing configuration file, creating...");
        File file = SPIFFS.open(CONFIG_FILE_PATH, FILE_WRITE, true);
        _initLocalConfigurationFile(file);
        file.close();
        Serial.println("Local configuration file created");
    }
    if (!SPIFFS.exists(CONNECT_FILE_PATH)) {
        Serial.println("Missing connectivity file, creating...");
        File file = SPIFFS.open(CONNECT_FILE_PATH, FILE_WRITE, true);
        _initLocalConnectivityFile(file);
        file.close();
        Serial.println("Local connectivity file created");
    }
}

void ConfigHandler::_initLocalConfigurationFile(File& file) {
    memset(&configuration, 0x00, sizeof(configuration_t));
    file.write((uint8_t*)&configuration, sizeof(configuration_t));
}

void ConfigHandler::_initLocalConnectivityFile(File& file) {
    memset(&connectivity, 0x00, sizeof(connectivity_t));
    file.write((uint8_t*)&connectivity, sizeof(connectivity_t));
}

ConfigHandler configHandler;
