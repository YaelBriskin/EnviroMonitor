#include "sensors/Inc/DHT22.h"
#include "sensors/Inc/SensorBase.h"
#include <iostream>

int main() 
{
    Sensors::DHT22Sensor dht22(GPIO_DHT22);
    if (!dht22.open()) 
    {
        std::cerr << "Failed to initialize DHT22 sensor!" << std::endl;
        return 1;
    }
    Sensors::SensorData data;
    if (dht22.read(data)) 
    {
        std::cout << "Temperature: " << data.temperature << " C" << std::endl;
        std::cout << "Humidity: " << data.humidity << " %" << std::endl;
    } else 
    {
        std::cerr << "Failed to read data from DHT22!" << std::endl;
    }

    dht22.close();
    return 0;
}