#include "utils.h"
#include <chrono>

using namespace std;

#include <glog/logging.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

uint64_t utils::currentTime()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

std::string utils::generateUUID()
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return boost::lexical_cast<std::string>(uuid);
}

string utils::getDateTime()
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", now);
    return  string(buffer);
}
