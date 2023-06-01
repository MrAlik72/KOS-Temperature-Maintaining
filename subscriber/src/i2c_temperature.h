#ifndef BMP180_H
#define BMP180_H

#include <stdlib.h>
#include <stdio.h>
#include <i2c/i2c.h>
#include <unistd.h>

#define BMP180_I2C_ADDRESS 0x77
#define BMP180_CTRL_MEAS_REG 0xF4
#define BMP180_TEMP_REG 0xF6

int SetBspConfig(const char *i2cChannelName, const char *i2cPmuxConfig);
I2cHandle InitI2cChannel(const char *i2cChannelName);
I2cError WriteByteToRegister(I2cHandle handle, uint8_t regAddr, uint8_t data);
I2cError ReadBytesFromRegister(I2cHandle handle, uint8_t regAddr, uint8_t *data, size_t numBytes);
float ReadTemperature(I2cHandle handle);

#endif /* BMP180_H */
