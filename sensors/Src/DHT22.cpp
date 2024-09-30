#include "DHT22.h"

/*
* DHT22 data protocol:
*
* The first 5 bytes contain humidity, temperature and checksum information:
*
* 1st byte (byte 0): high byte for humidity.
* 2nd byte (byte 1): low byte for humidity.
* 3rd byte (byte 2): high byte for temperature.
* 4th byte (byte 3): low byte for temperature.
* 5th byte (byte 4): checksum, to check data integrity.
*
* Data format:
* - Humidity (two bytes) - 0.1% in whole percent
* - Temperature (two bytes) - 0.1°C in whole degrees
* - Checksum - the sum of the first four bytes (humidity and temperature)
*
*/
namespace Sensors
{

    DHT22Sensor::DHT22Sensor(int gpioPin) : SensorBase(), gpio(gpioPin, Periferia::Direction::Input) {} // Pass the pin to the GPIO constructor

    bool DHT22Sensor::open()
    {
        if (!gpio.open(O_WRONLY))
        {
            std::cerr << "Failed to open GPIO for writing" << std::endl;
            return false;
        }
        // Set low level (0) for 18ms
        gpio.write(0);
        std::this_thread::sleep_for(std::chrono::milliseconds(18));
        // Set high level (1) for 40ms
        gpio.write(1);
        std::this_thread::sleep_for(std::chrono::microseconds(40));
        // Waiting for response from DHT22
        std::this_thread::sleep_for(std::chrono::microseconds(80));
        std::this_thread::sleep_for(std::chrono::microseconds(80));
        // Check if the bus is high
        if (gpio.read() == 0)
        {
            return false;
        }
        return true;
    }

    bool DHT22Sensor::read(SensorData &data)
    {
        std::cout << "Reading data from DHT22" << std::endl;

        resetSignal();

        uint8_t buffer[MAX_BUFFER] = {0};
        size_t bytesRead = 0;
        // Read 5 bytes of data from the sensor
        for (size_t i = 0; i < MAX_BUFFER; ++i)
        {
            // Read one byte from GPIO and write it to the buffer
            buffer[i] = static_cast<uint8_t>(gpio.read());
            bytesRead++;
        }
        if (bytesRead < MAX_BUFFER)
        {
            std::cerr << "Not enough data read from GPIO" << std::endl;
            return false;
        }
        // Checksum verification
        uint8_t checksum = (buffer[0] + buffer[1] + buffer[2] + buffer[3]) & 0xFF;
        if (checksum != buffer[4])
        {
            std::cerr << "Checksum error!" << std::endl;
            return false;
        }
        // Humidity calculation
        data.humidity = ((buffer[0] << 8) | buffer[1]) * 0.1f;
        // Temperature calculation

        int16_t rawTemperature = ((buffer[2] & 0x7F) << 8) | buffer[3];

        // if the high bit is set, the temperature is negative
        if (buffer[2] & 0x80)
        {
            rawTemperature = -rawTemperature;
        }
        // result in degrees Celsius
        data.temperature = rawTemperature * 0.1f;
        std::cout << "temperature: " << data.temperature << " C, humidity: " << data.humidity << " %" << std::endl;
        return true;
    }

    /**
     * @brief Closes the GPIO associated with the DHT22 sensor.
     */
    void DHT22Sensor::close()
    {
        gpio.close();
        std::cout << "Closing DHT22" << std::endl;
    }
} // namespace Sensors
