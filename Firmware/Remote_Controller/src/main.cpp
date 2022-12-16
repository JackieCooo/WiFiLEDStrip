#include <Arduino.h>
#include <lvgl.h>

#include "display.h"
#include "indev.h"
#include "main_gui.h"

Display display;
Indev indev;
// FocalTech_Class touch = FocalTech_Class(FT6236_SDA, FT6236_SCL);

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
}

void loop(void) {
  lv_task_handler();
  lv_tick_inc(5);
  delay(5);
  // if (touch.getTouched()) {
  //   uint16_t x, y;
  //   touch.getPoint(x, y);
  //   Serial.printf("x: %d, y: %d\n", x, y);
  // }
  // delay(500);
}
