#pragma once

#include <Arduino.h>
#include <SPIFFS.h>

#include "global.h"

#define CONFIG_FILE_PATH        "/configuration.txt"
#define WIFI_CONFIG_FILE_PATH   "/connectivity.txt"

class ConfigHandler {
public:
    ConfigHandler();
    void begin(void);
    void load(void);
    void save(void);
    void process(void);
    static void task(void* args);
private:
    void _initConfigurationSetting(File& file);
    void _initConnectivitySetting(File& file);
    void _checkLocalFiles(void);
};

extern ConfigHandler configHandler;
