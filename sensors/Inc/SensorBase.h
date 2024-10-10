#ifndef SENSOR_BASE_H
#define SENSOR_BASE_H


namespace Sensors {

struct SensorData {
    float temperature;
    float humidity;
    float light;
};
// Base class for sensor interface
class SensorBase 
{
public:
// Virtual destructor to ensure proper cleanup of derived classes
    virtual ~SensorBase() = default;
    // Initialize the sensor
    virtual bool open() = 0;
    // Read data from the sensor
    virtual bool read(SensorData& data) = 0;
    // Close the sensor and release resources
    virtual void close() = 0;
};

} // namespace Sensors


#endif // SENSOR_BASE_H