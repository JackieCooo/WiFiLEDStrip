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
    if (!SPIFFS.exists(CONFIG_FILE_PATH)) {
        Serial.println("Local config file not find, creating");
        File file = SPIFFS.open(CONFIG_FILE_PATH, FILE_WRITE, true);
        file.close();
        Serial.println("Local config file created");
    }
    else {
        File file = SPIFFS.open(CONFIG_FILE_PATH, FILE_READ);
        if (file) {
            Serial.println("Loading configuration");
            uint8_t buf[sizeof(configuration_t)];
            memset(buf, 0x00, sizeof(buf));
            file.read(buf, sizeof(configuration_t));
            memcpy(&configuration, buf, sizeof(configuration_t));
            file.close();
            _validateConfiguration();
        }
        else {
            Serial.println("File open failed");
        }
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

void ConfigHandler::_validateConfiguration(void) {
    if (configuration.setting.normal.color.full == 0) {
        configuration.setting.normal.color = DEFAULT_COLOR;
    }
    if (configuration.setting.breathing.color.full == 0) {
        configuration.setting.breathing.color = DEFAULT_COLOR;
    }
    if (configuration.setting.lightbeam.color.full == 0) {
        configuration.setting.lightbeam.color = DEFAULT_COLOR;
    }
}

ConfigHandler configHandler;
