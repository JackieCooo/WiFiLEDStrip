#include "FileLoader.h"

bool FileLoader::load(const char* filepath, uint8_t* buf, uint16_t len) {
    if (!SPIFFS.exists(filepath)) {
        return false;
    }

    File file = SPIFFS.open(filepath, FILE_READ);
    if (file) {
        file.read(buf, len);
        file.close();
        return true;
    }
    return false;
}

bool FileLoader::save(const char* filepath, uint8_t* buf, uint16_t len) {
    File file;

    if (!SPIFFS.exists(filepath)) {
        file = SPIFFS.open(filepath, FILE_WRITE, true);
    }
    else {
        file = SPIFFS.open(filepath, FILE_WRITE);
    }
    
    if (file) {
        file.write(buf, len);
        file.close();
        return true;
    }
    return false;
}
