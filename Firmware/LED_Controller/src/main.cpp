#include <Arduino.h>

#include "StripHandler.h"
#include "ConnectHandler.h"
#include "ConfigHandler.h"
#include "message_handler.h"
#include "global.h"

void setup() {
  Serial.begin(115200);

  msg_init();
  configHandler.begin();
  configHandler.load();
  stripHandler.begin();
  connHandler.begin();
}

void loop() {
  stripHandler.process();
  connHandler.process();
  configHandler.process();
}
