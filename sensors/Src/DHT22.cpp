#include "../Inc/DHT22.h"

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
    /**
     * @brief Constructor for the DHT22Sensor class.
     *
     * Initializes the DHT22 sensor with the specified GPIO pin.
     *
     * @param gpioPin The GPIO pin number to which the sensor is connected.
     */
    DHT22Sensor::DHT22Sensor(int gpioPin) : SensorBase(), gpio(gpioPin, OUTPUT)
    {
    }

    /**
     * @brief Opens the GPIO for writing to the DHT22 sensor.
     *
     * This method prepares the GPIO pin for communication with the sensor.
     * If the GPIO cannot be opened for writing, an error message is printed.
     *
     * @return true if the GPIO is opened successfully, false otherwise.
     */
    bool DHT22Sensor::open()
    {
        if (!gpio.open(O_WRONLY))
        {
            printf("Failed to open GPIO for writing\n");
            return false;
        }
        return true;
    }


    void DHT22Sensor::delay(int time, const std::string &unit)
    {
        if (unit == MS)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(time));
        }
        else if (unit == US)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(time));
        }
        else
        {
            printf("Error: Unsupported time unit code.\n");
        }
    }
    /**
     * @brief Resets the DHT22 signal.
     *
     * This method sends a start signal to the DHT22 sensor, waits for its response,
     * and checks if the sensor is ready to communicate.
     *
     * @return ERROR if there is an issue with the communication, OK otherwise.
     */
    int DHT22Sensor::startSignal()
    {
        // Set low level (0) for 18ms to start communication
        if (gpio.write(LOW) == ERROR) 
        {
            printf("Failed to set GPIO low.\n");
            return ERROR;
        }
        delay(18,MS);

        // Set high level (1) for 20-40us for DHT22 to detect the start signal
        if (gpio.write(HIGH) == ERROR)
        {
            printf("Failed to set GPIO high.\n");
            return ERROR;
        }
        delay(30,US);
        
        
        // Step 2: Set direction to input for reading response
        if(gpio.setDirection(INPUT) == FAILED)
        {
            printf("Failed to set GPIO direction.\n");
            return ERROR;
        }
        usleep(10);

        
        // Step 3: Wait for DHT22 response signal
        delay(80,US);

        // Check if the bus is high, indicating DHT22 is responding
        if (gpio.read() != LOW)
        {
            printf("DHT22 not responding!\n");
            return ERROR; 
        }

        // Step 4: Wait for the next signal from DHT22
        delay(80 , US);
        
        if (gpio.read() == HIGH)
        {
            printf("DHT22 started to send data!\n");
        }
        else
        {
            printf("Incorrect response from DHT22!\n");
            return ERROR; 
        }
        return OK;
    }
    /**
     * @brief Reads data from the DHT22 sensor.
     *
     * This method reads 5 bytes of data from the sensor, calculates the humidity
     * and temperature values, and verifies the checksum.
     *
     * @param data A reference to the SensorData object to store the read values.
     * @return true if data is read successfully, false otherwise.
     */
    bool DHT22Sensor::read(SensorData &data)
    {
        // Reset the signal before reading data
        if (startSignal() == ERROR)
        {
            printf("Failed to start signal.\n");
            return false;
        }
        uint8_t buffer[DHT22_DATA_BYTE_COUNT] = {0};

        // Read 5 bytes (40 bits) of data from the sensor
        for (size_t i = 0; i < DHT22_DATA_BIT_COUNT; ++i)
        {
            // Measure signal duration to determine bit value
            int bit = measure_signal_duration(); // Wait for HIGH signal
            if (bit == ERROR)
            {
                printf("Error: Failed to read bit!\n");
                return false; // Error reading bit
            }
            // Store the bit in the buffer
            buffer[i / 8] <<= 1; // Shift left for the new bit
            if (bit == HIGH)
            {
                buffer[i / 8] |= 1; // Set bit to 1
            }

            // Wait for 50us before reading the next bit
            //std::this_thread::sleep_for(std::chrono::microseconds(50));
            delay(DELAY_BETWEEN_BITS,US);
        }

        // Checksum verification
        uint8_t checksum = (buffer[0] + buffer[1] + buffer[2] + buffer[3]) & 0xFF;
        if (checksum != buffer[4])
        {
            printf("Checksum error!\n");
            return false; // Checksum validation failed
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
        printf("Temperature: %.2f C, Humidity: %.2f %%\n", data.temperature, data.humidity);
        return true;
    }
    /**
     * @brief Measures the duration of a signal from the DHT22 sensor.
     *
     * This method waits for the LOW signal to start measuring,
     * then waits for the HIGH signal and calculates its duration.
     *
     * @return LOW if the measured signal corresponds to bit 0,
     *         HIGH if the measured signal corresponds to bit 1,
     *         ERROR if there is a timeout or measurement issue.
     */
    int DHT22Sensor::measure_signal_duration()
    {
        struct timespec start, end;
        int value = -1;

        // Record the initial time
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Wait for the LOW signal (start of bit transmission)
        while (gpio.read() != LOW)
        {
            clock_gettime(CLOCK_MONOTONIC, &end);
            long elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 +
                                (end.tv_nsec - start.tv_nsec) / 1000;

            if (elapsed_time > TIMEOUT_US)
            {
                return ERROR; // Timeout occurred
            }
        }

        // Wait for high level and measure its duration
        clock_gettime(CLOCK_MONOTONIC, &start);
        // Wait until the signal becomes HIGH
        while (gpio.read() == LOW)
            ;

        clock_gettime(CLOCK_MONOTONIC, &end);
        long elapsed_time = (end.tv_sec - start.tv_sec) * 1000000 +
                            (end.tv_nsec - start.tv_nsec) / 1000;

        // Check the duration of the HIGH signal to determine bit value
        if (elapsed_time >= LOW_THRESHOLD_US_MIN && elapsed_time <= LOW_THRESHOLD_US_MAX)
        {
            return LOW; // Bit 0
        }
        else if (elapsed_time >= HIGH_THRESHOLD_US && elapsed_time <= TIMEOUT_US)
        {
            return HIGH; // Bit 1
        }

        return ERROR; // Measurement issue
    }
    /**
     * @brief Closes the GPIO associated with the DHT22 sensor.
     */
    void DHT22Sensor::close()
    {
        gpio.close();
        printf("Closing DHT22\n");
    }
} // namespace Sensors
