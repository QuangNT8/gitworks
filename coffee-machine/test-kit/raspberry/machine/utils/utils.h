#ifndef UTILS_H_
#define UTILS_H_
#include <iostream>
class utils
{
public:
    static uint64_t currentTime();
    static std::string generateUUID();    
    static std::string getDateTime();
};

#endif /* UTILS_H_ */
