#pragma once

#include <Arduino.h>
#include <SPIFFS.h>

class FileLoader {
public:
    bool load(const char* filepath, uint8_t* buf, uint16_t len);
    bool save(const char* filepath, uint8_t* buf, uint16_t len);
};
