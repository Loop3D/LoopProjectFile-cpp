#ifndef __LOOPGEOPHYSICALMODELS_H
#define __LOOPGEOPHYSICALMODELS_H

#include "LoopProjectFileUtils.h"
#include "LoopExtents.h"

namespace LoopProjectFile {

namespace GeophysicalModels {
/*!
 * \brief Checks the structure of the given netCDF root node for the
 * geophysical models and checks that the dimensions within that structure match the project file
 * extent attributes.
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param xyzGridSize - the size of each dimension for the data (x=easting, y=northing, z=depth)
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return A flag indicating whether the netCDF file is valid for geophysical structure and dimension sizes
 */
bool CheckGeophysicalModelsValid(netCDF::NcGroup* rootNode, std::vector<int> xyzGridSize, bool verbose=false);

/*!
 * \brief Adds or overrides geophysical model data into the loop project file at a
 * specific index location
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param data - the data to add
 * \param dataShape - the dimensions of the data being added
 * \param index - the index location for the data
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of data insert with error message if it failed
 */
LoopProjectFileResponse SetGeophysicalModel(netCDF::NcGroup* rootNode, std::vector<float> data, std::vector<int> dataShape, unsigned int index=0, bool verbose=false);

/*!
 * \brief Retrieves geophysical model data from the loop project file at a
 * specific index location
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param data - a reference to where the data is to be copied
 * \param dataShape - the dimensions of the data being retrieved
 * \param index - the index location for the data
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of data retrieval with error message if it failed
 */
LoopProjectFileResponse GetGeophysicalModel(netCDF::NcGroup* rootNode, std::vector<float>& data, std::vector<int>& dataShape, unsigned int index=0, bool verbose=false);

} // namespace GeophysicalModels
} // namespace LoopProjectFile

#endif