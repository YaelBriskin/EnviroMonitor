#include "sensors/Inc/DHT22.h"
#include "sensors/Inc/SensorBase.h"
#include <iostream>

int main() 
{
    Sensors::DHT22Sensor dht22(GPIO_DHT22);

    Sensors::SensorData data;
    if (dht22.read(data)) 
    {
        printf("Temperature: %.2f C\n", data.temperature);
        printf("Humidity: %.2f %%\n", data.humidity);
    } 
    else 
    {
        printf("Failed to read data from DHT22!\n");
    }
    return 0;
}