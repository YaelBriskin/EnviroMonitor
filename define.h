#ifndef DEFINE_H
#define DEFINE_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

#define MAX_PATH 50
#define MAX_DIRECTION_PATH 50
#define PIN_BUFF 4

#define ERROR -1
/**
 * @brief Status_t is an enumeration for the status of GPIO operations.
 */
typedef enum {
    FAILED = 0,  ///< Operation failed
    SUCCESS = 1  ///< Operation succeeded
} Status_t;

#endif // DEFINE_H
