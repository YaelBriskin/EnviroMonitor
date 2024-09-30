#ifndef GL5516_SENSOR_H
#define GL5516_SENSOR_H

#include "SensorBase.h"
#include <iostream>
#include "../periferia/Inc/gpio.h"

namespace Sensors {

class GL5516Sensor : public SensorBase 
{
public:

 explicit GL5516Sensor(int gpioPin);
    // Инициализация фоторезистора
    bool init() override;

    // Чтение уровня света
    bool read(SensorData& data) override;

    // Закрытие соединения с фоторезистором
    void close() override ;

private:
    int gpioPin;
};

} // namespace Sensors

#endif // GL5516_SENSOR_H
