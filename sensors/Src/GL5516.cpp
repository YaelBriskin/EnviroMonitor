// #include "../Inc/GL5516.h"
// #include <iostream> 

// namespace Sensors {

// /**
//  * @brief Constructor for the GL5516 sensor.
//  * 
//  * @param gpioPin The GPIO pin number the sensor is connected to.
//  */
// GL5516Sensor::GL5516Sensor(int gpioPin) : gpioPin(gpioPin) {}

// /**
//  * @brief Initializes the GL5516 sensor.
//  * 
//  * This function sets up the necessary configurations for the sensor 
//  * and prepares it for operation.
//  * 
//  * @return true if initialization is successful, false otherwise.
//  */
// bool GL5516Sensor::open() {
//     std::cout << "Initializing GL5516 sensor on GPIO pin " << gpioPin << std::endl;
//     // Add GPIO initialization code here if required
//     return true;
// }

// /**
//  * @brief Reads light intensity from the GL5516 sensor.
//  * 
//  * This function reads the current light intensity value from the sensor 
//  * and stores it in the provided SensorData structure.
//  * 
//  * @param data A reference to a SensorData structure where the light 
//  *             intensity value will be stored.
//  * @return true if reading is successful, false otherwise.
//  */
// bool GL5516Sensor::read(SensorData& data) {
//     std::cout << "Reading light intensity from GL5516 sensor on GPIO pin " << gpioPin << std::endl;
//     int lightValue = gpio.read();
//     data.light = static_cast<float>(lightValue);
//     std::cout << "Light Level: " << data.light << std::endl;
//     return true;
// }

// /**
//  * @brief Closes the GL5516 sensor and releases resources.
//  * 
//  * This function performs necessary cleanup operations for the sensor, 
//  * such as releasing GPIO pins or other resources used during its 
//  * operation.
//  */
// void GL5516Sensor::close() {
//     std::cout << "Closing GL5516 sensor on GPIO pin " << gpioPin << std::endl;
//     // Add code for closing/releasing GPIO if required
// }

// } // namespace Sensors