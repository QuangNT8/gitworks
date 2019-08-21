#include "utils.h"
#include <chrono>

using namespace std;

#include <glog/logging.h>

uint64_t utils::currentTime()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}
