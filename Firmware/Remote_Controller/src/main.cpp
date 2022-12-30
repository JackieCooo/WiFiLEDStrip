#include <Arduino.h>

#include "lvgl.h"
#include "display.h"
#include "indev.h"
#include "main_gui.h"
#include "ConnectHandler.h"

Display display;
Indev indev;

void setup(void) {
  Serial.begin(115200);

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
  create_main_gui();
  Serial.println("gui init done");

  connHandler.begin();
}

void loop(void) {
  lv_task_handler();
  lv_tick_inc(5);
  connHandler.process();
  delay(5);
}
