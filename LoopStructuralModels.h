#ifndef __LOOPSTRUCTURALMODELS_H
#define __LOOPSTRUCTURALMODELS_H

#include "LoopProjectFileUtils.h"
#include "LoopExtents.h"

namespace LoopProjectFile {

struct StructuralModelsConfiguration {
    char foliationInterpolator[30];
    int  foliationNumElements;
    double foliationBuffer;
    char foliationSolver[30];
    int foliationDamp;

    char faultInterpolator[30];
    int faultNumElements;
    double faultDataRegion;
    char faultSolver[30];
    int faultCpw;
    int faultNpw;

    StructuralModelsConfiguration() {
        for (int i=0; i<30; i++) {
            foliationInterpolator[i] = 0;
            foliationSolver[i] = 0;
            faultInterpolator[i] = 0;
            faultSolver[i] = 0;
        }
        strncpy_s(foliationInterpolator,"PLI",3);
        foliationNumElements = 100000;
        foliationBuffer = 0.8;
        strncpy_s(foliationSolver,"pyamg",5);
        foliationDamp = 1;

        strncpy_s(faultInterpolator,"FDI",3);
        faultNumElements = 30000;
        faultDataRegion = 0.3;
        strncpy_s(faultSolver,"pyamg",5);
        faultCpw = 10;
        faultNpw = 10;
    };
};

namespace StructuralModels {

/*!
 * \brief Checks the structure of the given netCDF root node for the
 * structural geology models and checks that the dimensions within that structure match the project file
 * extent attributes.
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param xyzGridSize - the size of each dimension for the data (x=easting, y=northing, z=depth)
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return A flag indicating whether the netCDF file is valid for structural geology structure and dimension sizes
 */
bool CheckStructuralModelsValid(netCDF::NcGroup* rootNode, std::vector<int> xyzGridSize, bool verbose=false);

/*!
 * \brief Adds or overrides structual geology model data into the loop project file at a
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
LoopProjectFileResponse SetStructuralModel(netCDF::NcGroup* rootNode, std::vector<float> data, std::vector<int> dataShape, unsigned int index=0, bool verbose=false);

/*!
 * \brief Retrieves structural geology model data from the loop project file at a
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
LoopProjectFileResponse GetStructuralModel(netCDF::NcGroup* rootNode, std::vector<float>& data, std::vector<int>& dataShape, unsigned int index=0, bool verbose=false);

/*!
 * \brief Retrieves structural model configuration from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param configuration - a reference to where the configuration data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetStructuralModelsConfiguration(netCDF::NcGroup* rootNode, StructuralModelsConfiguration& configuration, bool verbose=false);

/*!
 * \brief Sets structural model configuration to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param configuration - the configuration to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetStructuralModelsConfiguration(netCDF::NcGroup* rootNode, StructuralModelsConfiguration configuration, bool verbose=false);
} // namespace StructuralModels
} // namespace LoopProjectFile

#endif