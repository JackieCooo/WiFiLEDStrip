#include <Arduino.h>

#include "lvgl.h"
#include "display.h"
#include "indev.h"
#include "./gui/gui.h"
#include "global.h"
#include "message_handler.h"
#include "ConnectHandler.h"
// #include "ConfigHandler.h"

Display display;
Indev indev;

void setup(void) {
  Serial.begin(115200);

  initMessageHandler();

  Serial.println("lvgl init");
  lv_init();
  Serial.println("lvgl init done");

  Serial.println("display init");
  display.init();
  Serial.println("display init done");

  Serial.println("touch init");
  indev.init();
  Serial.println("touch init done");

  Serial.println("gui init");
  init_gui();
  Serial.println("gui init done");

  configHandler.begin();
  configHandler.load();

  Serial.println("connect init");
  connHandler.begin();
  Serial.println("connect init done");

  xTaskCreate(ConnectHandler::task, "ConnectHandlerTask", 4096, NULL, 5, NULL);
  // xTaskCreate(ConfigHandler::task, "ConfigHandlerTask", 4096, NULL, 4, NULL);
}

void loop(void) {
  lv_task_handler();
  lv_tick_inc(5);
  delay(5);
}
