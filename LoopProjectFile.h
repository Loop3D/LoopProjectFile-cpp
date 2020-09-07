#ifndef __LOOPPROJECTFILE__H
#define __LOOPPROJECTFILE__H

#include <netcdf>
#include <string>
#include <iostream>
#include <sys/stat.h>

#include "LoopVersion.h"
#include "LoopExtents.h"
#include "LoopDataCollection.h"
#include "LoopExtractedInformation.h"
#include "LoopProjectFileUtils.h"
#include "LoopStructuralModels.h"
#include "LoopGeophysicalModels.h"
#include "LoopUncertaintyModels.h"

/*! \brief The core namespace for Loop Project File functions and structures */
namespace LoopProjectFile {

/*!
 * \brief Creates a simple loop project file with versioning information 
 *
 * \param filename - the filename of the loop project file to create
 *
 * \return Response with success/fail of the create of the file with error message if it failed
 */
LoopProjectFileResponse CreateBasicFile(std::string filename);

/*! 
 * \brief Opens a loop project file
 *
 * \param filename - the filename of the loop project file to open
 * \param file - a reference to return the netCDF file once opened
 * \param readOnly - a flag to open the file in read only mode 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return A flag to indicate whether the project file failed 0-Success, 1-Failed
 */
bool OpenProjectFile(std::string filename, netCDF::NcFile& file, bool readOnly=true, bool verbose=false);

/*! 
 * \brief Closes an open netCDF file with appropriate exception handling and error messaging
 *
 * \param file - a pointer to the netCDF file to be closed 
 */
void CloseProjectFile(netCDF::NcFile* file);

/*!
 * \brief Checks the structure of the loop project file is valid
 *
 * \param filename - the name of the file to check
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return  A flag indicating whether the netCDF file is valid
 */
bool CheckFileValid(std::string filename, bool verbose=false);

/*!
 * \brief Sets the version of this repo to the project file
 *
 * \param filename - the name of the file to change
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of setting version with an error message if it failed
 */
LoopProjectFileResponse SetVersion(std::string filename, bool verbose=false);

/*!
 * \brief Gets the version of the repo used to create the project file
 *
 * \param filename - the name of the file to retrieve the version from
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return The version of the repo used to create this project file
 */
LoopVersion GetVersion(std::string filename, bool verbose=false);

// Getters for Extents/Observation/Events/Layers/Models

/*! @{
 * \brief Retrieves specified data from the loop project file
 *
 * \param filename - the filename of the loop project file
 * \param data - a reference to where the data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of data retrieval with error message if it failed
 */
LoopProjectFileResponse GetExtents(std::string filename, LoopExtents& data, bool verbose=false);
LoopProjectFileResponse GetFaultObservations(std::string filename, std::vector<FaultObservation> &data, bool verbose=false);
LoopProjectFileResponse GetFoldObservations(std::string filename, std::vector<FoldObservation> &data, bool verbose=false);
LoopProjectFileResponse GetFoliationObservations(std::string filename, std::vector<FoliationObservation> &data, bool verbose=false);
LoopProjectFileResponse GetDiscontinuityObservations(std::string filename, std::vector<DiscontinuityObservation> &data, bool verbose=false);
LoopProjectFileResponse GetFaultEvents(std::string filename, std::vector<FaultEvent> &data, bool verbose=false);
LoopProjectFileResponse GetFoldEvents(std::string filename, std::vector<FoldEvent> &data, bool verbose=false);
LoopProjectFileResponse GetFoliationEvents(std::string filename, std::vector<FoliationEvent> &data, bool verbose=false);
LoopProjectFileResponse GetDiscontinuityEvents(std::string filename, std::vector<DiscontinuityEvent> &data, bool verbose=false);
LoopProjectFileResponse GetStratigraphicLayers(std::string filename, std::vector<StratigraphicLayer> &data, bool verbose=false);
/*!@}*/

/*! @{
 * \brief Retrieves specified data from the loop project file
 *
 * \param filename - the filename of the loop project file
 * \param data - a reference to where the data is to be copied
 * \param dataShape - the dimensions of the data being retrieved
 * \param index - the index location for the data
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of data retrieval with error message if it failed
 */
LoopProjectFileResponse GetStructuralModel(std::string filename, std::vector<float> &data, std::vector<int> &dataShape, int index, bool verbose=false);
LoopProjectFileResponse GetGeophysicalModel(std::string filename, std::vector<float> &data, std::vector<int> &dataShape, int index, bool verbose=false);
LoopProjectFileResponse GetUncertaintyModel(std::string filename, std::vector<float> &data, std::vector<int> &dataShape, int index, bool verbose=false);
/*!@}*/

// Setters for Extents/Observation/Events/Layers/Models

/*! @{
 * \brief Adds or overrides specified data to the loop project file
 *
 * \param filename - the filename of the loop project file
 * \param data - the data to be added
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of data insertion with error message if it failed
 */
LoopProjectFileResponse SetExtents(std::string filename, LoopExtents data, bool verbose=false);
LoopProjectFileResponse SetFaultObservations(std::string filename, std::vector<FaultObservation> data, bool verbose=false);
LoopProjectFileResponse SetFoldObservations(std::string filename, std::vector<FoldObservation> data, bool verbose=false);
LoopProjectFileResponse SetFoliationObservations(std::string filename, std::vector<FoliationObservation> data, bool verbose=false);
LoopProjectFileResponse SetDiscontinuityObservations(std::string filename, std::vector<DiscontinuityObservation> data, bool verbose=false);
LoopProjectFileResponse SetFaultEvents(std::string filename, std::vector<FaultEvent> data, bool verbose=false);
LoopProjectFileResponse SetFoldEvents(std::string filename, std::vector<FoldEvent> data, bool verbose=false);
LoopProjectFileResponse SetFoliationEvents(std::string filename, std::vector<FoliationEvent> data, bool verbose=false);
LoopProjectFileResponse SetDiscontiuityEvents(std::string filename, std::vector<DiscontinuityEvent> data, bool verbose=false);
LoopProjectFileResponse SetStratigraphicLayers(std::string filename, std::vector<StratigraphicLayer> data, bool verbose=false);
/*!@}*/

/*! @{
 * \brief Adds or overrides specified data to the loop project file
 *
 * \param filename - the filename of the loop project file
 * \param data - the data to be added
 * \param dataShape - the dimensions of the data being retrieved
 * \param index - the index location for the data
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of data insertion with error message if it failed
 */
LoopProjectFileResponse SetStructuralModel(std::string filename, std::vector<float> data, std::vector<int> dataShape, int index, bool verbose=false);
LoopProjectFileResponse SetGeophysicalModel(std::string filename, std::vector<float> data, std::vector<int> dataShape, int index, bool verbose=false);
LoopProjectFileResponse SetUncertaintyModel(std::string filename, std::vector<float> data, std::vector<int> dataShape, int index, bool verbose=false);
/*!@}*/

} // namespace LoopProjectFile


#endif