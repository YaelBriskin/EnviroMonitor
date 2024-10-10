#include "../Inc/gpio.h"

namespace Periferia
{
    /**
     * @brief Constructs a GPIO object for a specific pin and direction.
     * @param pinNumber The GPIO pin number.
     * @param direction The direction of the GPIO pin ("in" or "out").
     */
    GPIO::GPIO(int pinNumber, int direction)
        : pin(pinNumber), gpio_fd(-1), direction(direction)
    {
        printf("Initializing GPIO pin %d with direction %s\n", pinNumber, (direction == INPUT ? "in" : "out"));

        // Calling the init method in the constructor
        if (init() != SUCCESS)
        {
            printf("Failed to initialize GPIO pin.\n");
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
                printf("Error opening GPIO export: %s\n", strerror(errno));
                return FAILED;
            }

            // Write the pin number to the export file
            char pinBuffer[PIN_BUFF];
            snprintf(pinBuffer, PIN_BUFF, "%d", pin);
            if (::write(export_fd, pinBuffer, strlen(pinBuffer)) == ERROR)
            {
                printf("Error writing pin number to export: %s\n", strerror(errno));
                ::close(export_fd);
                return FAILED;
            }
            printf("Exported GPIO_%d.\n", pin);
            ::close(export_fd);
        }
        else
        {
            printf("GPIO_%d already exported.\n", pin);
        }
        // Set the direction of the GPIO pin
        snprintf(directionPath, MAX_DIRECTION_PATH, "/sys/class/gpio/gpio%d/direction", pin);
        gpio_fd = ::open(directionPath, O_WRONLY);
        if (gpio_fd == ERROR)
        {
            printf("Error opening GPIO direction: %s\n", strerror(errno));
            return FAILED;
        }
        const char *dirStr = (direction == INPUT) ? "in" : "out";
        // Write the direction to the direction file ("in" or "out")
        if (::write(gpio_fd, dirStr, strlen(dirStr)) == ERROR)
        {
            printf("Error writing direction to GPIO: %s\n", strerror(errno));
            close();
            return FAILED;
        }
        printf("Set GPIO_%d direction to %s.\n", pin, dirStr);
        close();

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
            printf("Error opening GPIO value file: %s\n", strerror(errno));
        }
        return gpio_fd;
    }

    int GPIO::write(int value)
    {
        if(open(O_WRONLY) == ERROR)
        {
            printf("Failed to open GPIO pin before writing.\n");
            return ERROR;
        }
        char val_str = (value == 1) ? '1' : '0';
        if (::write(gpio_fd, &val_str, 1) == ERROR)
        {
            printf("Error writing GPIO value : %s\n", strerror(errno));
            close();
            return ERROR;
        }
        close();
        printf("Wrote value %d to GPIO pin %d.\n", value, pin);
        return OK;
    }
    /**
     * @brief Reads the value from the GPIO pin.
     * @return 1 if the GPIO value is high ('1'), 0 if low ('0').
     */
    int GPIO::read()
    {
        if(open(O_RDONLY) == ERROR)
        {
            printf("Failed to open GPIO pin before reading\n");
            return ERROR;
        }
        char value;
        if (::read(gpio_fd, &value, sizeof(value)) == ERROR)
        {
            printf("Error reading GPIO value: %s\n", strerror(errno));
            close();
            return ERROR;
        }
        int readValue = (value == '1') ? 1 : (value == '0' ? 0 : -1);
        if (readValue == -1)
        {
            printf("Unexpected value read from GPIO_%d: %d\n", pin, value);        
        }
        else
        {
            printf("Read [%d] from GPIO_%d.\n", readValue, pin);
        }
        close();
        return readValue;
    }

    /**
     * @brief Closes the GPIO pin by closing its file descriptor.
     */
    void GPIO::close()
    {
        if (gpio_fd != -1)
        {
            ::close(gpio_fd);
            //printf("Closed GPIO pin %d.\n", pin);
            gpio_fd = -1;
        }
    }

    /**
     * @brief Sets the direction of the GPIO pin.
     * @param newDirection The new direction for the GPIO pin (Input or Output).
     * @return SUCCESS if the direction is successfully set, FAILED otherwise.
     */
    Status_t GPIO::setDirection(int newDirection )
    {
        // Check if the new direction is different from the current direction
        if (direction == newDirection)
        {
            return SUCCESS; // No need to change if the direction is already set
        }

        // Set the direction in the file system
        char directionPath[MAX_DIRECTION_PATH];
        snprintf(directionPath, MAX_DIRECTION_PATH, "/sys/class/gpio/gpio%d/direction", pin);
        gpio_fd = ::open(directionPath, O_WRONLY);
        if (gpio_fd == ERROR)
        {
            printf("Error opening GPIO direction: %s\n", strerror(errno));
            return FAILED;
        }

        const char *dirStr = (newDirection == INPUT) ? "in" : "out";
        // Write the new direction to the direction file
        if (::write(gpio_fd, dirStr, strlen(dirStr)) == ERROR)
        {
            printf("Error writing direction to GPIO: %s\n", strerror(errno));
            close();
            return FAILED;
        }

        close();
        direction = newDirection; // Update the current direction
        printf("Set GPIO_%d direction to %s.\n", pin, dirStr);

        return SUCCESS;
    }
} // namespace Peripheria