#pragma once

#include <Arduino.h>
#include <SPIFFS.h>

#include "FileLoader.h"
#include "StripHandler.h"
#include "NormalFeature.h"
#include "BreathingFeature.h"
#include "LightbeamFeature.h"
#include "RainbowFeature.h"
#include "global.h"

#define CONFIG_FILE_PATH        "/config.bin"
#define CONNECT_FILE_PATH       "/connect.bin"

typedef struct {
    uint8_t power;
    uint8_t mode;
    struct {
        struct {
            uint16_t color;
        } normal;
        struct {
            uint16_t color;
            uint16_t duration;
            uint16_t interval;
            uint8_t ease;
        } breathing;
        struct {
            uint16_t color;
            uint8_t len;
            uint8_t gap;
            uint8_t fade;
            uint8_t head;
            uint8_t tail;
            uint8_t dir;
            uint16_t speed;
        } lightbeam;
        struct {
            uint16_t speed;
        } rainbow;
    } setting;
} config_t;

class FileManager : public FileLoader {
public:
    void begin(void);
    void checkFileIntergrality(void);
    void initConfigurationFile(config_t& file);
    void initConnectivityFile(connectivity_t& file);
    void saveConfiguration(void);
    void loadConfiguration(void);

private:
    void _dump_buf(uint8_t* buf, uint16_t len) {
#if DEBUG_LOG
        Serial.print("Buf: ");
        for(uint16_t i = 0; i < len; i++) {
            Serial.printf("%02X ", buf[i]);
        }
        Serial.println();
#endif
    }
};

extern FileManager fileManager;
