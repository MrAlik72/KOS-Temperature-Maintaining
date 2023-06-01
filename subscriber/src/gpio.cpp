#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <gpio/gpio.h>
#include <stdbool.h>
#include <string>
#include "gpiomodule.h"

#ifdef __arm__
#include <bsp/bsp.h>
#endif

#define GPIO_PIN_NUM_2 2U

#define BUF_SIZE 1024
#define HIGH 1
#define LOW 0

void on(GpioHandle* handle){
    GpioOut(*handle, GPIO_PIN_NUM_2, HIGH);
}

void off(GpioHandle* handle){
    GpioOut(*handle, GPIO_PIN_NUM_2, LOW);
}

int gpiomain(const std::string& cmd)
{

#ifdef __arm__

    {
        BspError rc = BspInit(NULL);
        if (rc != BSP_EOK)
        {
            fprintf(stderr, "Failed to initialize BSP\n");
            return EXIT_FAILURE;
        }

        rc = BspSetConfig("gpio0", "raspberry_pi4b.default");
        if (rc != BSP_EOK)
        {
            fprintf(stderr, "Failed to set mux configuration for gpio0 module\n");
            return EXIT_FAILURE;
        }
    }
#endif

    if (GpioInit())
    {
        fprintf(stderr, "GpioInit failed\n");
        return EXIT_FAILURE;
    }

    GpioHandle handle = NULL;
    if (GpioOpenPort("gpio0", &handle) || handle == GPIO_INVALID_HANDLE)
    {
        fprintf(stderr, "GpioOpenPort failed\n");
        return EXIT_FAILURE;
    }

    GpioSetMode(handle, GPIO_PIN_NUM_2, GPIO_DIR_OUT);

    GpioHandle* p_handle = &handle;

    if(cmd=="On")
    {
      on(p_handle);

    }
    if(cmd=="Off")
    {
      off(p_handle);
    }

    if(GpioClosePort(handle))
    {
        fprintf(stderr, "GpioClosePort failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
