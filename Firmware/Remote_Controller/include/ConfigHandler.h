#pragma once

#include <Arduino.h>
#include <SPIFFS.h>

#include "global.h"

#define CONFIG_FILE_PATH        "/configuration.bin"
#define CONNECT_FILE_PATH       "/connectivity.bin"
#define SETTING_FILE_PATH       "/setting.bin"

class ConfigHandler {
public:
    void begin(void);
    void load(void);
    void save(local_file_t cmd);
    void process(void);
    static void task(void* args);
private:
    void _initConfigurationSetting(File& file);
    void _initConnectivitySetting(File& file);
    void _checkLocalFiles(void);
};

extern ConfigHandler configHandler;
