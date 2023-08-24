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

void setup() {
    Serial.begin(115200);

    initMessageHandler();

    lv_init();
    display.init();
    indev.init();
    init_gui();
    Serial.println("GUI init done");

    //  configHandler.begin();
    //  configHandler.load();

    connHandler.begin();

    xTaskCreate(ConnectHandler::task, "ConnectHandlerTask", 4096, nullptr, 5, nullptr);
    // xTaskCreate(ConfigHandler::task, "ConfigHandlerTask", 4096, NULL, 4, NULL);
}

void loop() {
  lv_task_handler();
  lv_tick_inc(5);
  delay(5);
}
