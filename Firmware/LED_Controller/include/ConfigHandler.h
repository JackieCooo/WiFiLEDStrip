#include <Arduino.h>
#include <SPIFFS.h>

#include "global.h"

#define CONFIG_FILE_PATH        "/config.txt"

class ConfigHandler {
public:
    void begin(void);
    void load(void);
    void save(void);
private:
    
};

extern ConfigHandler configHandler;
