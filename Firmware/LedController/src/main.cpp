#include <Arduino.h>

#include "StripHandler.h"
#include "ConnectHandler.h"
#include "FileManager.h"
#include "global.h"

void setup() {
    Serial.begin(115200);

    fileManager.begin();
    connHandler.begin();

    xTaskCreate(StripHandler::task, "StripHandlerTask", 4096, nullptr, 5, nullptr);
    xTaskCreate(ConnectHandler::task, "ConnectHandlerTask", 4096, nullptr, 6, nullptr);
}

void loop() {

}
