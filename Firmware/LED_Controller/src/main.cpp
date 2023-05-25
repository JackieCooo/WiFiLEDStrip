#include <Arduino.h>

#include "StripHandler.h"
#include "ConnectHandler.h"
#include "FileManager.h"
#include "global.h"

void setup() {
  Serial.begin(115200);

  fileManager.begin();
  connHandler.begin();

  xTaskCreate(StripHandler::task, "StripHandlerTask", 4096, NULL, 5, NULL);
  xTaskCreate(ConnectHandler::task, "ConnectHandlerTask", 4096, NULL, 6, NULL);
}

void loop() {
  
}
