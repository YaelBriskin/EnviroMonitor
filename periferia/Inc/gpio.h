#ifndef GPIO_H
#define GPIO_H

#include "../define.h"

#define GPIO_DHT22 60
namespace Periferia
{
    enum class Direction
    {
        Input,
        Output
    };
    /**
     * @class GPIO
     * @brief Class for handling GPIO operations such as initialization, reading, and closing the GPIO pin.
     */
    class GPIO
    {
    public:
        // Constructor: takes a pin number and direction
        explicit GPIO(int pinNumber, Direction direction);
        // Destructor for automatic closing of the pin
        ~GPIO();

        // Initialize GPIO
        Status_t init();
        // Open GPIO pin
        int open(int flag);
        // Writing value to GPIO
        void write(int value);
        // Reading value from GPIO
        int read();
        // Close GPIO pin
        void close();
        int getPinNumber() const { return pin; }

    private:
        int pin;             ///< GPIO pin number
        int gpio_fd;         ///< File descriptor for the GPIO
        Direction direction; ///< Direction of the GPIO pin
    };
} // namespace Periferia

#endif // GPIO_H