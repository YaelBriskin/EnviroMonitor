#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H

#include "SensorBase.h"
#include "../periferia/Inc/gpio.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

#define MAX_BUFFER 5

namespace Sensors
{

    //// Define a DHT22Sensor class that inherits from SensorBase
    class DHT22Sensor : public SensorBase
    {
    public:
        // DHT22Sensor class constructor, accepts GPIO pin number
        explicit DHT22Sensor(int gpioPin);
        // Initialize the sensor, overrides the virtual method from SensorBase
        bool open() override;
        // Reading data from the sensor: temperature and humidity
        bool read(SensorData& data) override; 
        // Close the sensor
        void close() override;
    private:
        Periferia::GPIO gpio; // Object for working with GPIO
        void resetSignal();
    };

} // namespace Sensors

#endif // DHT22_SENSOR_H