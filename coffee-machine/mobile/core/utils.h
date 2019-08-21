#ifndef UTILS_H
#define UTILS_H

#include <chrono>

class utils
{
public:
    static uint64_t current_time()
    {
        auto duration = std::chrono::system_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

};

#endif // UTILS_H

