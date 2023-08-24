#ifndef _CST816D_H
#define _CST816D_H

#include <Arduino.h>
#include <Wire.h>

#define CST816D_SLAVE_ADDRESS               (0x15)
#define CST816D_CHIP_ID                     (0xB6)
#define CST816D_I2C_SPEED                   (400000)

#define CST816D_REG_MODE                    (0x00)
#define CST816D_REG_GESTURE                 (0x01)
#define CST816D_REG_FINGER_NUM              (0x02)
#define CST816D_REG_XH                      (0x03)
#define CST816D_REG_XL                      (0x04)
#define CST816D_REG_YH                      (0x05)
#define CST816D_REG_YL                      (0x06)
#define CST816D_REG_BPC0H                   (0xB0)
#define CST816D_REG_BPC0L                   (0xB1)
#define CST816D_REG_BPC1H                   (0xB2)
#define CST816D_REG_BPC1L                   (0xB3)
#define CST816D_REG_CHIP_ID                 (0xA7)
#define CST816D_REG_PROJ_ID                 (0xA8)
#define CST816D_REG_FW_VER                  (0xA9)
#define CST816D_REG_MOTION_MASK             (0xEC)
#define CST816D_REG_TRQ_PLUSE_WIDTH         (0xED)
#define CST816D_REG_NOR_SCAN_PER            (0xEE)
#define CST816D_REG_MOTION_SL_ANGLE         (0xEF)
#define CST816D_REG_LP_SCAN_RAW_1H          (0xF0)
#define CST816D_REG_LP_SCAN_RAW_1L          (0xF1)
#define CST816D_REG_LP_SCAN_RAW_2H          (0xF2)
#define CST816D_REG_LP_SCAN_RAW_2L          (0xF3)
#define CST816D_REG_LP_SCAN_TH              (0xF5)
#define CST816D_REG_LP_SCAN_WIN             (0xF6)
#define CST816D_REG_LP_SCAN_FREQ            (0xF7)
#define CST816D_REG_LP_SCAN_IDAC            (0xF8)
#define CST816D_REG_AUTO_SLEEP_TIME         (0xF9)
#define CST816D_REG_IRQ_CTL                 (0xFA)
#define CST816D_REG_AUTO_RESET              (0xFB)
#define CST816D_REG_LONG_PRESS_TIME         (0xFC)
#define CST816D_REG_IO_CTL                  (0xFD)
#define CST816D_REG_DIS_AUTO_SLEEP          (0xFE)


class CST816D_Class {

public:
    CST816D_Class(int8_t sda = -1, int8_t scl = -1);
    bool begin(TwoWire &port = Wire, uint8_t addr = CST816D_SLAVE_ADDRESS);
    uint8_t getChipID(void);
    bool getPoint(uint16_t &x, uint16_t &y);
    bool getTouched(void);

private:
    bool probe(void);
    uint8_t readRegister8(uint8_t reg);
    void writeRegister8(uint8_t reg, uint8_t value);
    bool readBytes(uint8_t reg, uint8_t *data, uint8_t nbytes);
    bool writeBytes(uint8_t reg, uint8_t *data, uint8_t nbytes);

    bool initialized = false;
    uint8_t _address;
    TwoWire *_i2cPort;

};

#endif
