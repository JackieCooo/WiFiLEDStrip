#include <Arduino.h>

#include "StripHandler.h"
#include "ConnectHandler.h"
#include "ConfigHandler.h"
#include "global.h"

void setup() {
  Serial.begin(115200);

  // configHandler.begin();
  // configHandler.load();
  connHandler.begin();

  xTaskCreate(StripHandler::task, "StripHandlerTask", 4096, NULL, 5, NULL);
  xTaskCreate(ConnectHandler::task, "ConnectHandlerTask", 4096, NULL, 6, NULL);
  // xTaskCreate(ConfigHandler::task, "ConfigHandlerTask", 4096, NULL, 4, NULL);
}

void loop() {
  
}
