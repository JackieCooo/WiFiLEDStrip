#include "ConfigHandler.h"

configuration_t configuration;

void ConfigHandler::begin(void) {
    if (SPIFFS.begin(true)) {
        Serial.println("SPIFFS init done");
    }
    else {
        Serial.println("SPIFFS init failed");
    }
}

void ConfigHandler::load(void) {
    if (!SPIFFS.exists(CONFIG_FILE_PATH)) {
        File file = SPIFFS.open(CONFIG_FILE_PATH, FILE_WRITE, true);
        file.close();
    }
    File file = SPIFFS.open(CONFIG_FILE_PATH, FILE_READ);
    if (file) {
        Serial.println("Reading configuration");
        uint8_t buf[sizeof(configuration_t)];
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
        Serial.println("Writing configuration");
        file.write((uint8_t*)&configuration, sizeof(configuration_t));
        file.close();
    }
    else {
        Serial.println("File open failed");
    }
}

ConfigHandler configHandler;
