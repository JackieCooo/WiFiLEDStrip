#include "CST816D.h"

CST816D_Class::CST816D_Class(int8_t sda, int8_t scl) {
    Wire.setPins(sda, scl);
    Wire.begin(sda, scl, CST816D_I2C_SPEED);
}

bool CST816D_Class::begin(TwoWire &port, uint8_t addr) {
    _i2cPort = &port;
    _address = addr;
    if (!probe()) return false;
    writeRegister8(CST816D_REG_MODE, 0x00);  // Normal mode
    writeRegister8(CST816D_REG_DIS_AUTO_SLEEP, 0x01);  // Disable low power mode
    return true;
}

bool CST816D_Class::probe(void) {
    uint8_t vid = readRegister8(CST816D_REG_CHIP_ID);
    Serial.printf("VID: 0x%x\n", vid);
    if (vid == CST816D_CHIP_ID) {
        initialized = true;
        return true;
    }
    return false;
}

bool CST816D_Class::readBytes(uint8_t reg, uint8_t *data, uint8_t nbytes) {
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(reg);
    _i2cPort->endTransmission();
    _i2cPort->requestFrom(_address, nbytes);
    uint8_t index = 0;
    while (_i2cPort->available())
        data[index++] = _i2cPort->read();
    return nbytes == index;
}

bool CST816D_Class::writeBytes(uint8_t reg, uint8_t *data, uint8_t nbytes) {
    _i2cPort->beginTransmission(_address);
    _i2cPort->write(reg);
    for (uint8_t i = 0; i < nbytes; i++) {
        _i2cPort->write(data[i]);
    }
    return _i2cPort->endTransmission() != 0;
}

uint8_t CST816D_Class::readRegister8(uint8_t reg) {
    uint8_t value = 0x00;
    (void)readBytes(reg, &value, 1);
    return value;
}

void CST816D_Class::writeRegister8(uint8_t reg, uint8_t value) {
    (void)writeBytes(reg, &value, 1);
}

uint8_t CST816D_Class::getChipID(void) {
    if (!initialized) return 0;
    return readRegister8(CST816D_REG_CHIP_ID);
}

bool CST816D_Class::getTouched(void) {
    if (!initialized) return false;
    return readRegister8(CST816D_REG_FINGER_NUM);
}

bool CST816D_Class::getPoint(uint16_t &x, uint16_t &y) {
    if (!initialized) return false;
    uint8_t buf[5];
    if (readBytes(CST816D_REG_FINGER_NUM, buf, 5)) {
        if (buf[0] == 0 || buf[0] > 2) {
            return false;
        }
        x = (buf[1] & 0x0F) << 8 | buf[2];
        y = (buf[3] & 0x0F) << 8 | buf[4];
        return true;
    }
    return false;
}
