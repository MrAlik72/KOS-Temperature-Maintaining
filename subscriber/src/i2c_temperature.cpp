#include "i2c_temperature.h"
#include "general.h"
#include <bsp/bsp.h>
#include <coresrv/hal/hal_api.h>

#include <rtl/stdbool.h>
#include <rtl/countof.h>

#define SLAVE_ADDR 0x77
#define I2C_FREQ   100000

#define BUF_SIZE 1024

#define BMP180_CTRL_MEAS_REG 0xF4
#define BMP180_TEMP_REG      0xF6

int SetBspConfig(const char *i2cChannelName, const char *i2cPmuxConfig)
{
    Retcode rc                      = rcFail;
    char    buf[BUF_SIZE]           = {0};
    char    boardName[BSP_NAME_MAX] = {0};

    rc = BspInit(NULL);

    if (rc != rcOk)
    {
        fprintf(stderr, "Failed to initialize BSP\n");
        return EXIT_FAILURE;
    }

    rc = BspEnableModule(i2cChannelName);
    if (rc != rcOk)
    {
        fprintf(stderr, "Failed to enable %s module\n", i2cChannelName);
        return EXIT_FAILURE;
    }

    if (KnHalGetEnv("board", boardName, sizeof(boardName)) != rcOk)
    {
        fprintf(stderr, "KnHalGetEnv() failed\n");
        return EXIT_FAILURE;
    }

    int ret = snprintf(buf, BUF_SIZE, "%s.%s", boardName, i2cPmuxConfig);
    if (ret < 0)
    {
        fprintf(stderr, "BSP config name generation failed\n");
        return EXIT_FAILURE;
    }

    rc = BspSetConfig(i2cChannelName, buf);
    if (rc != rcOk)
    {
        fprintf(stderr, "Failed to set pmux configuration for %s channel\n",
                i2cChannelName);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

I2cHandle InitI2cChannel(const char *i2cChannelName)
{
    I2cHandle handle        = NULL;
    char      buf[BUF_SIZE] = {0};
    I2cError  rc            = I2C_EFAIL;

    rc = I2cInit();

    if (I2C_FAIL(rc))
    {
        fprintf(stderr, "Failed to initialize I2C\n");
        return handle;
    }

    rc = I2cEnumChannels(0, sizeof(buf), buf);
    if (I2C_FAIL(rc))
    {
        fprintf(stderr, "Failed to enumerate I2C channels\n");
        return handle;
    }

    rc = I2cOpenChannel(i2cChannelName, &handle);
    if (I2C_FAIL(rc))
    {
        fprintf(stderr, "Failed to open the %s channel\n", i2cChannelName);
        return handle;
    }

    return handle;
}

I2cError WriteByteToRegister(I2cHandle handle, uint8_t regAddr, uint8_t data)
{
    I2cMsg i2cMsg;
    uint8_t cmdBuf[2];

    cmdBuf[0] = regAddr;
    cmdBuf[1] = data;

    i2cMsg.addr  = SLAVE_ADDR;
    i2cMsg.flags = 0;
    i2cMsg.buf   = cmdBuf;
    i2cMsg.len   = 2;

    return I2cXfer(handle, I2C_FREQ, &i2cMsg, 1);
}

I2cError ReadBytesFromRegister(I2cHandle handle, uint8_t regAddr, uint8_t *data, size_t numBytes)
{
    I2cMsg i2cMsg[2];
    uint8_t cmdBuf[1];

    cmdBuf[0] = regAddr;

    i2cMsg[0].addr  = SLAVE_ADDR;
    i2cMsg[0].flags = 0;
    i2cMsg[0].buf   = cmdBuf;
    i2cMsg[0].len   = 1;

    i2cMsg[1].addr  = SLAVE_ADDR;
    i2cMsg[1].flags = I2C_FLAG_RD;
    i2cMsg[1].buf   = data;
    i2cMsg[1].len   = numBytes;

    return I2cXfer(handle, I2C_FREQ, i2cMsg, 2);
}

float ReadTemperature(I2cHandle handle)
{
    // Start temperature measurement
    if (WriteByteToRegister(handle, BMP180_CTRL_MEAS_REG, 0x2E) != I2C_EOK) {
        fprintf(stderr, "Failed to start temperature measurement\n");
        return 0.0f;
    }

    // Wait for measurement to complete
    // Sleep for 5 milliseconds (adjust as needed)
    usleep(5000);

    // Read temperature data
    uint8_t tempData[2];
    if (ReadBytesFromRegister(handle, BMP180_TEMP_REG, tempData, 2) != I2C_EOK) {
        fprintf(stderr, "Failed to read temperature data\n");
        return 0.0f;
    }

    // Calculate temperature value
    int16_t rawTemp = (tempData[0] << 8) | tempData[1];
    float temperature = (float)rawTemp / 10.0f;

    return temperature;
}
