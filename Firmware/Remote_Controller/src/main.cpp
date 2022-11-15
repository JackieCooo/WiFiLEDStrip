#include <Arduino.h>
#include <Arduino_ST7789.h>
#include <focaltech.h>
#include <Wire.h>

#define ST7789_DC    -1
#define ST7789_RST   25
#define ST7789_CS    33   // only for displays with CS pin
#define ST7789_MOSI  26   // for hardware SPI data pin (all of available pins)
#define ST7789_SCLK  27   // for hardware SPI sclk pin (all of available pins)
#define ST7789_BLC   32

#define FT6236_SDA 22
#define FT6236_SCL 21

Arduino_ST7789 screen = Arduino_ST7789(ST7789_DC, ST7789_RST, ST7789_MOSI, ST7789_SCLK, ST7789_CS, ST7789_BLC);
FocalTech_Class touch = FocalTech_Class(FT6236_SDA, FT6236_SCL);
uint16_t x = 0, y = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.println("ST7789 init");
  screen.init();
  screen.fillScreen(BLUE);
  Serial.println("ST7789 init done");
  Serial.println("FT6236 init");
  touch.begin();
  Serial.printf("VID: 0x%x, VID1: 0x%x\n", touch.getVendorID(), touch.getVendor1ID());
  Serial.println("FT6236 init done");
}

void loop(void) {
}
