#include "LoopProjectFileUtils.h"
#include <string>
#include <iostream>

namespace LoopProjectFile {
 
LoopProjectFileResponse createErrorMsg(int errorCode, std::string errorMsg, bool echo)
{
    if (echo) std::cout << errorMsg << std::endl;
    LoopProjectFileResponse resp = {errorCode,errorMsg};
    return resp;
}

}; // namespace LoopProjectFile