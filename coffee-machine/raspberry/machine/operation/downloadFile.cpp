#include "operations.h"
#include "manager.h"
#include "config.h"
#include <fstream>

SERVICE_EXECUTE(machine, DownloadFile)
{
    DLOG(INFO)<< "Execute request DownloadFile";

    /* read data file */
    std::string filePath = TEMPORARY_PATH;
    filePath = filePath + "/" + request.name();
    std::ifstream fs(filePath, std::ios::binary);
    if(fs.fail())
    {
        fs.open("build/default/install-root/"+filePath, std::ios::binary);
        if(fs.fail())
        {
            DLOG(INFO)<<"file not exists";
            return comm::INTERNAL_ERROR;
        }
    }

    fs.seekg (0, fs.end);
    long length = fs.tellg();
    fs.seekg (0, fs.beg);

    if (length <= 0)
    {
        fs.close();
        return comm::INTERNAL_ERROR;
    }

    char * buffer = new char[length];
    if (!fs.read(buffer,length))
    {
        DLOG(INFO)<<"read file error";
        fs.close();
        return comm::INTERNAL_ERROR;
    }
    fs.close();

    /* process reply */
    reply.set_data(buffer, length);
    delete [] buffer;

    return 0;
}
