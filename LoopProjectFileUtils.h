#ifndef __LOOPPROJECTFILEUTILS_H
#define __LOOPPROJECTFILEUTILS_H


#include <netcdf>

/*! \brief A structure for returning an error code and message for loop project files */
struct LoopProjectFileResponse {
    int errorCode; /**< Error Code:- \n 0 - *No error* \n 1 - *Error* */
    std::string errorMessage; /**< Error message string to print/return */
};

namespace LoopProjectFile {
 
/*!
 * \brief Utility function to create a Loop Project File Response
 *
 * \param errorCode - the error code to add
 * \param errorMsg - the error message to add and opinionally print
 * \param echo - a flag to toggle printing message to standard out
 *
 * \return The created Loop Project File Response with error code and message
 */
LoopProjectFileResponse createErrorMsg(int errorCode, std::string errorMsg, bool echo=true)
{
    if (echo) std::cout << errorMsg << std::endl;
    LoopProjectFileResponse resp = {errorCode,errorMsg};
    return resp;
}

}; // namespace LoopProjectFile

#endif