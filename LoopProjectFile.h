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
LoopProjectFileResponse CreateBasicFile(std::string filename)
{
    LoopProjectFileResponse resp = {0,""};
    struct stat buffer;
    if (stat(filename.c_str(), &buffer) == 0) {
        resp = createErrorMsg(1,"File " + filename + " already exists");
    }
    netCDF::NcFile file(filename, netCDF::NcFile::replace, netCDF::NcFile::nc4);
    if (file.isNull()) {
        resp = createErrorMsg(1,"ERROR: Failure to open netCDF file " + filename);
    } else {
        LoopVersion::SetVersion(&file);
    }
    return resp;
}


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
bool OpenProjectFile(std::string filename, netCDF::NcFile& file, bool readOnly=true, bool verbose=false)
{
    if (verbose) std::cout << "Accessing file named: " << filename << std::endl;
    struct stat buffer;
    if (stat(filename.c_str(),&buffer) != 0) {
        if (verbose) std::cout << "File " << filename << " does not exist" << std::endl;
        return true;
    } else {
        file.open(filename,
            (readOnly?netCDF::NcFile::read : netCDF::NcFile::write),
            netCDF::NcFile::nc4);
        if (file.isNull()) {
            return true;
        }
    }
    return false;
}

/*! 
 * \brief Closes an open netCDF file with appropriate exception handling and error messaging
 *
 * \param file - a pointer to the netCDF file to be closed 
 */
void CloseProjectFile(netCDF::NcFile* file)
{
    try {
        if (!file->isNull() && file->isRootGroup()) file->close();
        else std::cout << "Trying to close an invalid or already closed project file" << std::endl;
    } catch (netCDF::exceptions::NcHdfErr &e) {
        std::cout << e.what() << std::endl;
    }
}

/*!
 * \brief Checks the structure of the loop project file is valid
 *
 * \param filename - the name of the file to check
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return  A flag indicating whether the netCDF file is valid
 */
bool CheckFileValid(std::string filename, bool verbose=false)
{
    bool valid = true;
    netCDF::NcFile file;
    if (!OpenProjectFile(filename, file, verbose)) {
        if (verbose) std::cout << "Checking validity of loop project file " << filename << std::endl;
        std::vector<int> xyzGridSize;
        valid = LoopVersion::CheckVersionValid(&file,verbose);
        valid = LoopExtents::CheckExtentsValid(&file,xyzGridSize,verbose);
        valid = DataCollection::CheckDataCollectionValid(&file,verbose);
        valid = ExtractedInformation::CheckExtractedInformationValid(&file,verbose);
        valid = StructuralModels::CheckStructuralModelsValid(&file,xyzGridSize,verbose);
        valid = GeophysicalModels::CheckGeophysicalModelsValid(&file,xyzGridSize,verbose);
        valid = UncertaintyModels::CheckUncertaintyModelsValid(&file,xyzGridSize,verbose);
    } else {
        valid = false;
    }
    return valid;
}

/*!
 * \brief Sets the version of this repo to the project file
 *
 * \param filename - the name of the file to amend
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of setting version with an error message if it failed
 */
LoopProjectFileResponse SetVersion(std::string filename, bool verbose=false)
{
    LoopProjectFileResponse resp;
    netCDF::NcFile file;
    if (!OpenProjectFile(filename, file, false, verbose)) {
        resp = LoopVersion::SetVersion(&file, verbose);
        CloseProjectFile(&file);
    } else {
        std::cout << "Failure to open project file " << filename << std::endl;
    }
    return resp;
}

/*!
 * \brief Gets the version of the repo used to create the project file
 *
 * \param filename - the name of the file to retrieve the version from
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return The version of the repo used to create this project file
 */
LoopVersion GetVersion(std::string filename, bool verbose=false)
{
    LoopVersion version;
    netCDF::NcFile file;
    if (!OpenProjectFile(filename, file, true, verbose)) {
        version = LoopVersion::GetVersion(&file, verbose);
        CloseProjectFile(&file);
    } else {
        std::cout << "Failure to open project file " << filename << std::endl;
    }
    return version;
}
/*****************************************************************************/
/*        FUNCTION MACROS USED TO STREAMLINE GETTER/SETTER FUNCTIONS         */
/*        ***SHOULD CHANGE TO TEMPLATES***                                   */
/*****************************************************************************/
#define LPF_OPEN_RUN(FILENAME,FUNCTION,CONTAINER,READONLY,VERBOSE) \
{\
    LoopProjectFileResponse resp = {0,""};\
    netCDF::NcFile file;\
    if (!OpenProjectFile(FILENAME, file, READONLY, VERBOSE)) {\
        resp = FUNCTION(&file, CONTAINER, VERBOSE);\
    } else {\
        std::cout << "Failure to open project file "  << filename << std::endl;\
    }\
    return resp;\
}

#define LPF_OPEN_RUN_WITH_SHAPE(FILENAME,FUNCTION,CONTAINER,SHAPE,INDEX,READONLY,VERBOSE) \
{\
    LoopProjectFileResponse resp = {0,""};\
    netCDF::NcFile file;\
    if (!OpenProjectFile(FILENAME, file, READONLY, VERBOSE)) {\
        resp = FUNCTION(&file, CONTAINER, SHAPE, INDEX, VERBOSE);\
    } else {\
        std::cout << "Failure to open project file "  << filename << std::endl;\
    }\
    return resp;\
}
/*****************************************************************************/

// Getters for Extents/Observation/Events/Models
/*! @{
 * \brief Retrieves specified data from the loop project file
 *
 * \param filename - the filename of the loop project file
 * \param data - a reference to where the data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of data retrieval with error message if it failed
 */
LoopProjectFileResponse GetExtents(std::string filename, LoopExtents& data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, LoopExtents::GetExtents, data, true, verbose);
}

LoopProjectFileResponse GetFaultObservations(std::string filename, std::vector<FaultObservation> &data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, DataCollection::GetFaultObservations, data, true, verbose);
}

LoopProjectFileResponse GetFoldObservations(std::string filename, std::vector<FoldObservation> &data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, DataCollection::GetFoldObservations, data, true, verbose);
}

LoopProjectFileResponse GetFoliationObservations(std::string filename, std::vector<FoliationObservation> &data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, DataCollection::GetFoliationObservations, data, true, verbose);
}

LoopProjectFileResponse GetDiscontinuityObservations(std::string filename, std::vector<DiscontinuityObservation> &data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, DataCollection::GetDiscontinuityObservations, data, true, verbose);
}

LoopProjectFileResponse GetFaultEvents(std::string filename, std::vector<FaultEvent> &data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::GetFaultEvents, data, true, verbose);
}

LoopProjectFileResponse GetFoldEvents(std::string filename, std::vector<FoldEvent> &data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::GetFoldEvents, data, true, verbose);
}

LoopProjectFileResponse GetFoliationEvents(std::string filename, std::vector<FoliationEvent> &data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::GetFoliationEvents, data, true, verbose);
}

LoopProjectFileResponse GetDiscontinuityEvents(std::string filename, std::vector<DiscontinuityEvent> &data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::GetDiscontinuityEvents, data, true, verbose);
}
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
LoopProjectFileResponse GetStructuralModel(std::string filename, std::vector<float> &data, std::vector<int> &dataShape, int index, bool verbose=false)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, StructuralModels::GetStructuralModel, data, dataShape, index, true, verbose);
}

LoopProjectFileResponse GetGeophysicalModel(std::string filename, std::vector<float> &data, std::vector<int> &dataShape, int index, bool verbose=false)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, GeophysicalModels::GetGeophysicalModel, data, dataShape, index, true, verbose);
}

LoopProjectFileResponse GetUncertaintyModel(std::string filename, std::vector<float> &data, std::vector<int> &dataShape, int index, bool verbose=false)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, UncertaintyModels::GetUncertaintyModel, data, dataShape, index, true, verbose);
}
/*!@}*/

// Setters for Extents/Observation/Events/Models

/*! @{
 * \brief Adds or overrides specified data to the loop project file
 *
 * \param filename - the filename of the loop project file
 * \param data - the data to be added
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of data insertion with error message if it failed
 */
LoopProjectFileResponse SetExtents(std::string filename, LoopExtents data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, LoopExtents::SetExtents, data, false, verbose);
}

LoopProjectFileResponse SetFaultObservations(std::string filename, std::vector<FaultObservation> data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, DataCollection::SetFaultObservations, data, false, verbose);
}

LoopProjectFileResponse SetFoldObservations(std::string filename, std::vector<FoldObservation> data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, DataCollection::SetFoldObservations, data, false, verbose);
}

LoopProjectFileResponse SetFoliationObservations(std::string filename, std::vector<FoliationObservation> data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, DataCollection::SetFoliationObservations, data, false, verbose);
}

LoopProjectFileResponse SetDiscontinuityObservations(std::string filename, std::vector<DiscontinuityObservation> data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, DataCollection::SetDiscontinuityObservations, data, false, verbose);
}

LoopProjectFileResponse SetFaultEvents(std::string filename, std::vector<FaultEvent> data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::SetFaultEvents, data, false, verbose);
}

LoopProjectFileResponse SetFoldEvents(std::string filename, std::vector<FoldEvent> data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::SetFoldEvents, data, false, verbose);
}

LoopProjectFileResponse SetFoliationEvents(std::string filename, std::vector<FoliationEvent> data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::SetFoliationEvents, data, false, verbose);
}
LoopProjectFileResponse SetDiscontinuityEvents(std::string filename, std::vector<DiscontinuityEvent> data, bool verbose=false)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::SetDiscontinuityEvents, data, false, verbose);
}
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
LoopProjectFileResponse SetStructuralModel(std::string filename, std::vector<float> data, std::vector<int> dataShape, int index, bool verbose=false)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, StructuralModels::SetStructuralModel, data, dataShape, index, false, verbose);
}

LoopProjectFileResponse SetGeophysicalModel(std::string filename, std::vector<float> data, std::vector<int> dataShape, int index, bool verbose=false)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, GeophysicalModels::SetGeophysicalModel, data, dataShape, index, false, verbose);
}

LoopProjectFileResponse SetUncertaintyModel(std::string filename, std::vector<float> data, std::vector<int> dataShape, int index, bool verbose=false)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, UncertaintyModels::SetUncertaintyModel, data, dataShape, index, false, verbose);
}
/*!@}*/

} // namespace LoopProjectFile


#endif