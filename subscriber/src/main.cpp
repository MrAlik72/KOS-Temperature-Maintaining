#include <cstring>
#include <exception>
#include <iostream>
#include <memory>


#include <platform/platform.h>
#include <kos_net.h>

#include <gpio/gpio.h>
#include <stdbool.h>

#if defined(__arm__) || defined(__aarch64__)
#include <bsp/bsp.h>
#endif
#include <rtl/countof.h>
#include <rtl/retcode_hr.h>


#include "gpiomodule.h"

#include "general.h"
#include "subscriber.h"

// should be global defines
#define prINF "INF"
#define prDBG "DBG"
#define prINI "INIT"
#define prERR "ERROR"
#define prWRN "WRN"

#define opOK "OK\n"
#define opFAIL "FAILED\n"


namespace consts {
constexpr const char *DefaultMqttAddress = "broker.emqx.io";
constexpr int DefaultMqttUnencryptedPort = 1883;
constexpr int PublicationIntervalInSec = 5;
} // namespace consts

double desiredTemperature = 25.0;

bool pidController(double currentTemperature, double desiredTemperature)
{
    return (desiredTemperature - currentTemperature) > 0;
}

static std::string GetBrokerAddress()
{
    const char *brokerAddress = getenv("MQTT_BROKER_ADDRESS");
    if (!brokerAddress)
    {
        std::cerr << app::AppTag
                  << "Failed to get MQTT broker address. Using default MQTT "
                     "broker address ("
                  << consts::DefaultMqttAddress << ")" << std::endl;
        return consts::DefaultMqttAddress;
    }
    return brokerAddress;
}

static int GetBrokerPort()
{
    const char *brokerPortEnvVariable = getenv("MQTT_BROKER_PORT");
    if (!brokerPortEnvVariable)
    {
        std::cerr << app::AppTag
                  << "Failed to get MQTT broker port. Using default MQTT "
                     "broker port ("
                  << consts::DefaultMqttUnencryptedPort << ")" << std::endl;
        return consts::DefaultMqttUnencryptedPort;
    }

    try
    {
        return std::stoi(brokerPortEnvVariable);
    }
    catch (const std::invalid_argument &ex)
    {
        std::cerr << app::AppTag
                  << "Failed to get MQTT broker port: " << ex.what()
                  << "Using default port ("
                  << consts::DefaultMqttUnencryptedPort << ")" << std::endl;
        return consts::DefaultMqttUnencryptedPort;
    }
}


void print(const char* msg, const char* prefix = nullptr)
{
    if(prefix) printf("[%s] %s\n", prefix, msg);
    else printf("%s", msg);
}


int main(void)
{
#if 0
    std::cerr << "Init gpio ??? "<< std::endl;

     gpioInitAll();

     std::cerr << "Run gpio ??? "<< std::endl;

     trajectoryCmd();

     std::cerr << "Finish gpio  "<< std::endl;
#endif



    if (!wait_for_network())
    {
        std::cerr << app::AppTag << "Error: Wait for network failed!"
                  << std::endl;
        return EXIT_FAILURE;
    }

    sleep(5);

    bool tumbler = 0;
    float currentTemperature = 20.0f;

    mosqpp::lib_init();

    auto sub = std::make_unique<Subscriber>(
        "subscriber", GetBrokerAddress().c_str(), GetBrokerPort());

    while (true)
    {
        tumbler = pidController(currentTemperature, desiredTemperature);

        fprintf(
            stderr,
            "Current temperature: %f, Desired temperature: %f, Tumbler %d\n",
            currentTemperature, desiredTemperature, tumbler
        );

        double offset = 0.1 + (rand() % 20 + 1) * 0.1;
        
        if(tumbler)
        {
            if(gpiomain("On") == EXIT_SUCCESS)
            {
                currentTemperature = currentTemperature + offset;
            }
        }
        else
        {
            if(gpiomain("Off") == EXIT_SUCCESS)
            {
                currentTemperature = currentTemperature - offset;
            }
        }

        if(sub)
        {
            sub->loop();
        }

        sleep(5);
    }
    

    mosqpp::lib_cleanup();
    return EXIT_SUCCESS;
}