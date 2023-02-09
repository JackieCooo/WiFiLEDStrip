#include <Arduino.h>
#include <SPIFFS.h>

#include "global.h"

#define CONFIG_FILE_PATH        "/config.bin"
#define CONNECT_FILE_PATH       "/connect.bin"

class ConfigHandler {
public:
    void begin(void);
    void load(void);
    void save(local_file_t cmd);
    void process(void);

    static void task(void* args);

private:
    void _checkFileIntergrality(void);
    void _initLocalConfigurationFile(File& file);
    void _initLocalConnectivityFile(File& file);
};

extern ConfigHandler configHandler;
