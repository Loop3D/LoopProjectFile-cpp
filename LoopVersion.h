#ifndef __LOOPVERSION_H
#define __LOOPVERSION_H

#define LoopVersionMajor 0
#define LoopVersionMinor 0
#define LoopVersionSub   14

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
static LoopProjectFileResponse SetVersion(netCDF::NcGroup* rootNode, bool verbose=false);

/*!
 * \brief Checks the version attributes of the Loop Project File are valid
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param verbose - a flag to toggle verbose message printing
 * 
 * \return A flag indicating whether the version is valid on the project file
 */
static bool CheckVersionValid(netCDF::NcGroup* rootNode, bool verbose=false);

/*!
 * \brief Gets the version attributes of the Loop Project File
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param verbose - a flag to toggle verbose message printing
 * 
 * \return The version of the project file
 */
static LoopVersion GetVersion(netCDF::NcGroup* rootNode, bool verbose=false);

};
} // namespace LoopProjectFile

#endif
