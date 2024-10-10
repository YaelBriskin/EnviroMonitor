#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H

#include "SensorBase.h"
#include "../../periferia/Inc/gpio.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <time.h>

#define DHT22_DATA_BIT_COUNT 40
#define DHT22_DATA_BYTE_COUNT 5
#define TIMEOUT_US 100000


#define HIGH_THRESHOLD_US 70
#define LOW_THRESHOLD_US_MIN 26
#define LOW_THRESHOLD_US_MAX 28

#define DELAY_BETWEEN_BITS 50

#define MS "ms"
#define US "us"

namespace Sensors
{

    //// Define a DHT22Sensor class that inherits from SensorBase
    class DHT22Sensor : public SensorBase
    {
    public:
        // DHT22Sensor class constructor, accepts GPIO pin number
        explicit DHT22Sensor(int gpioPin);
        
        void delay(int time, const std::string& unit);
        // Initialize the sensor, overrides the virtual method from SensorBase
        bool open() override;
        // Reading data from the sensor: temperature and humidity
        bool read(SensorData& data) override; 
        // Close the sensor
        void close() override;
    private:
        Periferia::GPIO gpio; // Object for working with GPIO
        int startSignal();
        int measure_signal_duration();
    };

} // namespace Sensors

#endif // DHT22_SENSOR_H