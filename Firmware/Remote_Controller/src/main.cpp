#include <Arduino.h>
#include <focaltech.h>
#include <Wire.h>
#include <lvgl.h>

#include "display.h"

#define FT6236_SDA 22
#define FT6236_SCL 21

Display display;
FocalTech_Class touch = FocalTech_Class(FT6236_SDA, FT6236_SCL);
uint16_t x = 0, y = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.println("lvgl init");
  lv_init();
  Serial.println("lvgl init done");
  Serial.println("ST7789 init");
  display.init();
  Serial.println("ST7789 init done");
  Serial.println("FT6236 init");
  touch.begin();
  Serial.printf("VID: 0x%x, VID1: 0x%x\n", touch.getVendorID(), touch.getVendor1ID());
  Serial.println("FT6236 init done");
  Serial.println("example init");

  Serial.println("example init done");
}

void loop(void) {
  lv_task_handler();
  delay(5);
}
