#include <Arduino.h>

#include "StripHandler.h"
#include "ConnectHandler.h"
#include "ConfigHandler.h"
#include "global.h"

void setup() {
  Serial.begin(115200);

  configHandler.begin();
  configHandler.load();
  stripHandler.begin();
  connHandler.begin();
}

void loop() {
  stripHandler.routine();
  connHandler.process();
}
