#pragma once

#include <Arduino.h>
#include <SPIFFS.h>

class FileLoader {
public:
    static bool load(const char* filepath, uint8_t* buf, uint16_t len);
    static bool save(const char* filepath, uint8_t* buf, uint16_t len);
};
