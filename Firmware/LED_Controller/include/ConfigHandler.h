#include <Arduino.h>
#include <SPIFFS.h>

#include "global.h"

#define CONFIG_FILE_PATH        "/config.txt"
#define WIFI_CONFIG_FILE_PATH   "/wifi.txt"

class ConfigHandler {
public:
    void begin(void);
    void load(void);
    void save(void);
    void process(void);
    static void task(void* args);
private:
};

extern ConfigHandler configHandler;
