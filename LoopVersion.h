#ifndef __LOOPVERSION_H
#define __LOOPVERSION_H

#define LoopVersionMajor 0
#define LoopVersionMinor 0
#define LoopVersionSub   1

#include <netcdf>
#include <string>
#include "LoopProjectFileUtils.h"

namespace LoopProjectFile {

/*! \brief A structure containing the version information of the project file */
struct LoopVersion {
    /*! @{ The segmented elements of the project file version */
    int majorVersion;
    int minorVersion;
    int subVersion;
    /*!@}*/
    bool errored; /*!<A flag indicating an error occured during collection of the version **0 = No Error**, **1 = Error** */
    /*! Constructor. Zeros all variables */
    LoopVersion() {
        majorVersion = 0;
        minorVersion = 0;
        subVersion = 0;
        errored = true;
    }

/*!
 * \brief Sets the version attributes of the Loop Project File to the version of this library
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param verbose - a flag to toggle verbose message printing
 * 
 * \return A flag indicating whether the version is set on the project file
 */
static LoopProjectFileResponse SetVersion(netCDF::NcGroup* rootNode, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    if (!rootNode) {
        resp = createErrorMsg(1,"ERROR: Root Node invalid!");
        return resp;
    }
    try {
        rootNode->putAtt("loopMajorVersion",netCDF::ncInt64,LoopVersionMajor);
        rootNode->putAtt("loopMinorVersion",netCDF::ncInt64,LoopVersionMinor);
        rootNode->putAtt("loopSubVersion",netCDF::ncInt64,LoopVersionSub);
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) e.what();
        resp = createErrorMsg(1,"ERROR: Failure to set version on netCDF file", verbose);
    }
    return resp;
}

/*!
 * \brief Checks the version attributes of the Loop Project File are valid
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param verbose - a flag to toggle verbose message printing
 * 
 * \return A flag indicating whether the version is valid on the project file
 */
static bool CheckVersionValid(netCDF::NcGroup* rootNode, bool verbose=false)
{
    LoopVersion version;
    if (!rootNode->isNull()) {
        return false;
    }
    try {
        rootNode->getAtt("loopMajorVersion").getValues(&version.majorVersion);
        rootNode->getAtt("loopMinorVersion").getValues(&version.minorVersion);
        rootNode->getAtt("loopSubVersion").getValues(&version.subVersion);
        if (verbose) {
            std::cout << "  Loop Project File version = "
                << version.majorVersion << "." << version.minorVersion << "." << version.subVersion
                << std::endl;
        }
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what();
        return false;
    }
    return true;
}

/*!
 * \brief Gets the version attributes of the Loop Project File
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param verbose - a flag to toggle verbose message printing
 * 
 * \return The version of the project file
 */
static LoopVersion GetVersion(netCDF::NcGroup* rootNode, bool verbose=false)
{
    LoopVersion version;
    if (rootNode->isNull()) {
        return version;
    }
    try {
        rootNode->getAtt("loopMajorVersion").getValues(&version.majorVersion);
        rootNode->getAtt("loopMinorVersion").getValues(&version.minorVersion);
        rootNode->getAtt("loopSubVersion").getValues(&version.subVersion);
        version.errored = false;
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) {
            e.what();
            std::cout << "No valid Version in Loop Project File" << std::endl;
            version.errored = true;
        }
    }
    return version;
}

};
} // namespace LoopProjectFile

#endif