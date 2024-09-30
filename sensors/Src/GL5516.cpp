#include "GL5516.h"
#include <iostream> 


namespace Sensors {
    GL5516Sensor::GL5516Sensor(int gpioPin) : gpioPin(gpioPin) {}
// Инициализация сенсора GL5516
bool GL5516Sensor::init() {
    std::cout << "Initializing GL5516 sensor on GPIO pin " << gpioPin << std::endl;
    // Добавь сюда код инициализации GPIO, если требуется
    return true;
}

// Чтение данных с сенсора (уровень освещенности)
bool GL5516Sensor::read(SensorData& data) 
{
    std::cout << "Reading light intensity from GL5516 sensor on GPIO pin " << gpioPin << std::endl;
    int lightValue = gpio.read();
    data.light=  static_cast<float>(lightValue);
    std::cout << "Light Level: " << data.light << std::endl;
    return true;
}

// Закрытие сенсора и освобождение ресурсов
void GL5516Sensor::close() 
{
    std::cout << "Closing GL5516 sensor on GPIO pin " << gpioPin << std::endl;
    // Добавь код для закрытия/освобождения GPIO, если требуется
}

} // namespace Sensors