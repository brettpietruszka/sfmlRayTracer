
#pragma once

#include <iostream>

// Change this value to turn on all of the logging/printing
#define DEBUG_ENABLED 0

/* Very simple debugging for now. I shouldn't need anything complex */
#if DEBUG_ENABLED

#define LOG_DEBUG(...)                      \
{                                           \
    std::fprintf(stderr, __VA_ARGS__);      \
    std::cerr << std::endl;                 \
}               

#define LOG_DEBUG_CHARARRAY(Arr) std::cerr << Arr << std::endl;

#else

#define LOG_DEBUG(...) (void*)0;
#define LOG_DEBUG_CHARARRAY(Arr) (void*)0;

#endif