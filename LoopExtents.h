#ifndef __LOOPEXTENTS_H
#define __LOOPEXTENTS_H

#include "LoopProjectFileUtils.h"
#include <netcdf>

namespace LoopProjectFile {

/*! \brief A structure containing boundary extents in both geodesic and UTM coordinate systems */
struct LoopExtents {
    /*!@{ The minimum and maximum extent boundaries in geodesic coordinates (degrees) */
    double minLatitude;
    double maxLatitude;
    double minLongitude;
    double maxLongitude; 
    /*!@}*/
    /*!@{ The minimum and maximum extent boundaries in UTM coordinates (metres) */
    double minNorthing;
    double maxNorthing;
    double minEasting;
    double maxEasting;
    /*!@}*/
    /*!@{ The UTM zone of the UTM coordinates */
    int utmZone;
    int utmNorthSouth;
    /*!@}*/
    /*!@{ The minimum and maximum extent depths (metres) */
    double minDepth;
    double maxDepth;
    /*!@}*/
    /*!@{ The spacing distances between points along each axis (metres) */
    double spacingX;
    double spacingY;
    double spacingZ;
    /*!@}*/
    /*!@{*/
    /*! The working format of these extents 0-UTM 1-Geodesic */
    int workingFormat;
    /*! A flag indicating an error occured during collection of these extents **0 = No Error**, **1 = Error** */
    bool errored;
    /*!@}*/
    /*! \brief Constructor. Zeros all variables */
    LoopExtents() {
        minLatitude = 0;
        maxLatitude = 0;
        minLongitude = 0;
        maxLongitude = 0;
        minNorthing = 0;
        maxNorthing = 0;
        minEasting = 0;
        maxEasting = 0;
        utmZone = 0;
        utmNorthSouth = 0;
        minDepth = 0;
        maxDepth = 0;
        spacingX = 0;
        spacingY = 0;
        spacingZ = 0;
        workingFormat = 0;
        errored = true;
    }

/*!
 * \brief Sets the extents within the Loop Project File referenced by the root node
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param extents - the extents structure to add to the file
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of data insert with error message if it failed
 */
static LoopProjectFileResponse SetExtents(netCDF::NcGroup* rootNode, LoopExtents extents, bool verbose=false);

/*!
 * \brief Gets the extents within the Loop Project File referenced by the root node
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param extents - the extents structure to retrieve from the file
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of data insert with error message if it failed
 */
static LoopProjectFileResponse GetExtents(netCDF::NcGroup* rootNode, LoopExtents& extents, bool verbose=false);

/*!
 * \brief Checks the extents within the Loop Project File referenced by the root node are valid
 *  and returns the grid size of the region of interest given the extents and spacing values
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param xyzGridSize - a reference to a vector of integers for returning the grid size of this project
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return A flag indicating whether the extents in the netCDF file are valid
 */
static bool CheckExtentsValid(netCDF::NcGroup* rootNode, std::vector<int>& xyzGridSize, bool verbose=false);

}; // LoopExtents
} // namespace LoopProjectFile

#endif