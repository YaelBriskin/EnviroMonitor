#include "gpio.h"

namespace Periferia
{
    /**
     * @brief Constructs a GPIO object for a specific pin and direction.
     * @param pinNumber The GPIO pin number.
     * @param direction The direction of the GPIO pin ("in" or "out").
     */
    GPIO::GPIO(int pinNumber, Direction direction)
        : pin(pinNumber), gpio_fd(-1), direction(direction)
    {
        // Calling the init method in the constructor
        if (init() != SUCCESS)
        {
            std::cerr << "Failed to initialize GPIO pin." << std::endl;
        }
    }

    /**
     * @brief Destroys the GPIO object and closes the GPIO pin if it is open.
     */
    GPIO::~GPIO()
    {
        if (gpio_fd != ERROR)
        {
            close();
        }
    }

    /**
     * @brief Initializes the GPIO pin by exporting it and setting its direction.
     * @return SUCCESS if initialization is successful, FAILED otherwise.
     */
    Status_t GPIO::init()
    {
        char gpioPath[MAX_PATH];
        char directionPath[MAX_DIRECTION_PATH];

        snprintf(gpioPath, MAX_PATH, "/sys/class/gpio/gpio%d", pin);

        // Check if the GPIO folder already exists
        if (access(gpioPath, F_OK) != 0)
        {
            // If the folder does not exist, export the GPIO pin
            int export_fd = ::open("/sys/class/gpio/export", O_WRONLY);
            if (export_fd == ERROR)
            {
                std::cerr << "Error opening GPIO export: " << strerror(errno) << std::endl;
                return FAILED;
            }

            // Write the pin number to the export file
            char pinBuffer[PIN_BUFF];
            snprintf(pinBuffer, PIN_BUFF, "%d", pin);
            if (::write(export_fd, pinBuffer, strlen(pinBuffer)) == ERROR)
            {
                std::cerr << "Error writing pin number to export: " << strerror(errno) << std::endl;
                ::close(export_fd);
                return FAILED;
            }
            ::close(export_fd);
        }

        // Set the direction of the GPIO pin
        snprintf(directionPath, MAX_DIRECTION_PATH, "/sys/class/gpio/gpio%d/direction", pin);
        gpio_fd = ::open(directionPath, O_WRONLY);
        if (gpio_fd == ERROR)
        {
            std::cerr << "Error opening GPIO direction: " << strerror(errno) << std::endl;
            return FAILED;
        }
        const char *dirStr = (direction == Direction::Input) ? "in" : "out";
        // Write the direction to the direction file ("in" or "out")
        if (::write(gpio_fd, dirStr, strlen(dirStr)) == ERROR)
        {
            std::cerr << "Error writing direction to GPIO: " << strerror(errno) << std::endl;
            ::close(gpio_fd);
            return FAILED;
        }
        ::close(gpio_fd);

        return SUCCESS;
    }
    /**
     * @brief Opens the GPIO value file for reading or writing.
     * @param flag Open mode (e.g., O_RDONLY or O_WRONLY).
     * @return File descriptor for the GPIO value file, or -1 if failed.
     */
    int GPIO::open(int flag)
    {
        char gpioPath[MAX_PATH];
        snprintf(gpioPath, MAX_PATH, "/sys/class/gpio/gpio%d/value", pin);
        gpio_fd = ::open(gpioPath, flag);
        if (gpio_fd == ERROR)
        {
            std::cerr << "Error opening GPIO value file: " << strerror(errno) << std::endl;
        }
        return gpio_fd;
    }

    void GPIO::write(int value)
    {
        char val_str = (value == 1) ? '1' : '0';
        if (::write(gpio_fd, &val_str, 1) == ERROR)
        {
            std::cerr << "Error writing GPIO value: " << strerror(errno) << std::endl;
            close();
            exit(EXIT_FAILURE);
        }
    }
    /**
     * @brief Reads the value from the GPIO pin.
     * @return 1 if the GPIO value is high ('1'), 0 if low ('0').
     */
    int GPIO::read()
    {
        char value;
        if (::read(gpio_fd, &value, sizeof(value)) == ERROR)
        {
            std::cerr << "Error reading GPIO value: " << strerror(errno) << std::endl;
            close();
            exit(EXIT_FAILURE);
        }
        return (value == '1') ? 1 : 0;
    }

    /**
     * @brief Closes the GPIO pin by closing its file descriptor.
     */
    void GPIO::close()
    {
        if (gpio_fd != -1)
        {
            ::close(gpio_fd);
            gpio_fd = -1;
        }
    }

} // namespace Peripheria