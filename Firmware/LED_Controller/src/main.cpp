#include <Arduino.h>
#include <StripHandler.h>
#include <ConnectHandler.h>

#include "global_conf.h"

void setup() {
  Serial.begin(115200);

  stripHandler.begin();
  connHandler.begin();
}

void loop() {
  stripHandler.routine();
  connHandler.process();
}
