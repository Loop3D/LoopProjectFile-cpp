#include "LoopProjectFile.h"

namespace LoopProjectFile {

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


bool OpenProjectFile(std::string filename, netCDF::NcFile& file, bool readOnly, bool verbose)
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

void CloseProjectFile(netCDF::NcFile* file)
{
    try {
        if (!file->isNull() && file->isRootGroup()) file->close();
        else std::cout << "Trying to close an invalid or already closed project file" << std::endl;
    } catch (netCDF::exceptions::NcHdfErr &e) {
        std::cout << e.what() << std::endl;
    }
}

bool CheckFileValid(std::string filename, bool verbose)
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

LoopProjectFileResponse SetVersion(std::string filename, bool verbose)
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

LoopVersion GetVersion(std::string filename, bool verbose)
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

LoopProjectFileResponse GetExtents(std::string filename, LoopExtents& data, bool verbose)
{
    LPF_OPEN_RUN(filename, LoopExtents::GetExtents, data, true, verbose);
}

LoopProjectFileResponse GetDataCollectionConfiguration(std::string filename, DataCollectionConfiguration& data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::GetDataCollectionConfiguration, data, true, verbose);
}

LoopProjectFileResponse GetDataCollectionSources(std::string filename, DataCollectionSources& data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::GetDataCollectionSources, data, true, verbose);
}

LoopProjectFileResponse GetStructuralModelsConfiguration(std::string filename, StructuralModelsConfiguration& data, bool verbose)
{
 LPF_OPEN_RUN(filename, StructuralModels::GetStructuralModelsConfiguration, data, true, verbose);
}

LoopProjectFileResponse GetFaultObservations(std::string filename, std::vector<FaultObservation> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::GetFaultObservations, data, true, verbose);
}

LoopProjectFileResponse GetFoldObservations(std::string filename, std::vector<FoldObservation> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::GetFoldObservations, data, true, verbose);
}

LoopProjectFileResponse GetFoliationObservations(std::string filename, std::vector<FoliationObservation> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::GetFoliationObservations, data, true, verbose);
}

LoopProjectFileResponse GetDiscontinuityObservations(std::string filename, std::vector<DiscontinuityObservation> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::GetDiscontinuityObservations, data, true, verbose);
}

LoopProjectFileResponse GetStratigraphicObservations(std::string filename, std::vector<StratigraphicObservation> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::GetStratigraphicObservations, data, true, verbose);
}

LoopProjectFileResponse GetFaultEvents(std::string filename, std::vector<FaultEvent> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::GetFaultEvents, data, true, verbose);
}

LoopProjectFileResponse GetFoldEvents(std::string filename, std::vector<FoldEvent> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::GetFoldEvents, data, true, verbose);
}

LoopProjectFileResponse GetFoliationEvents(std::string filename, std::vector<FoliationEvent> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::GetFoliationEvents, data, true, verbose);
}

LoopProjectFileResponse GetDiscontinuityEvents(std::string filename, std::vector<DiscontinuityEvent> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::GetDiscontinuityEvents, data, true, verbose);
}

LoopProjectFileResponse GetStratigraphicLayers(std::string filename, std::vector<StratigraphicLayer> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::GetStratigraphicLayers, data, true, verbose);
}

LoopProjectFileResponse GetEventRelationships(std::string filename, std::vector<EventLink> &data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::GetEventRelationships, data, true, verbose);
}

LoopProjectFileResponse GetStructuralModel(std::string filename, std::vector<float> &data, std::vector<int> &dataShape, int index, bool verbose)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, StructuralModels::GetStructuralModel, data, dataShape, index, true, verbose);
}

LoopProjectFileResponse GetGeophysicalModel(std::string filename, std::vector<float> &data, std::vector<int> &dataShape, int index, bool verbose)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, GeophysicalModels::GetGeophysicalModel, data, dataShape, index, true, verbose);
}

LoopProjectFileResponse GetUncertaintyModel(std::string filename, std::vector<float> &data, std::vector<int> &dataShape, int index, bool verbose)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, UncertaintyModels::GetUncertaintyModel, data, dataShape, index, true, verbose);
}

LoopProjectFileResponse SetExtents(std::string filename, LoopExtents data, bool verbose)
{
    LPF_OPEN_RUN(filename, LoopExtents::SetExtents, data, false, verbose);
}

LoopProjectFileResponse SetDataCollectionConfiguration(std::string filename, DataCollectionConfiguration data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::SetDataCollectionConfiguration, data, false, verbose);
}

LoopProjectFileResponse SetDataCollectionSources(std::string filename, DataCollectionSources data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::SetDataCollectionSources, data, false, verbose);
}

LoopProjectFileResponse SetStructuralModelsConfiguration(std::string filename, StructuralModelsConfiguration data, bool verbose)
{
    LPF_OPEN_RUN(filename, StructuralModels::SetStructuralModelsConfiguration, data, false, verbose);
}

LoopProjectFileResponse SetFaultObservations(std::string filename, std::vector<FaultObservation> data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::SetFaultObservations, data, false, verbose);
}

LoopProjectFileResponse SetFoldObservations(std::string filename, std::vector<FoldObservation> data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::SetFoldObservations, data, false, verbose);
}

LoopProjectFileResponse SetFoliationObservations(std::string filename, std::vector<FoliationObservation> data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::SetFoliationObservations, data, false, verbose);
}

LoopProjectFileResponse SetDiscontinuityObservations(std::string filename, std::vector<DiscontinuityObservation> data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::SetDiscontinuityObservations, data, false, verbose);
}

LoopProjectFileResponse SetStratigraphicObservations(std::string filename, std::vector<StratigraphicObservation> data, bool verbose)
{
    LPF_OPEN_RUN(filename, DataCollection::SetStratigraphicObservations, data, false, verbose);
}

LoopProjectFileResponse SetFaultEvents(std::string filename, std::vector<FaultEvent> data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::SetFaultEvents, data, false, verbose);
}

LoopProjectFileResponse SetFoldEvents(std::string filename, std::vector<FoldEvent> data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::SetFoldEvents, data, false, verbose);
}

LoopProjectFileResponse SetFoliationEvents(std::string filename, std::vector<FoliationEvent> data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::SetFoliationEvents, data, false, verbose);
}

LoopProjectFileResponse SetDiscontinuityEvents(std::string filename, std::vector<DiscontinuityEvent> data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::SetDiscontinuityEvents, data, false, verbose);
}

LoopProjectFileResponse SetStratigraphicLayers(std::string filename, std::vector<StratigraphicLayer> data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::SetStratigraphicLayers, data, false, verbose);
}

LoopProjectFileResponse SetEventRelationships(std::string filename, std::vector<EventLink> data, bool verbose)
{
    LPF_OPEN_RUN(filename, ExtractedInformation::SetEventRelationships, data, false, verbose);
}

LoopProjectFileResponse SetStructuralModel(std::string filename, std::vector<float> data, std::vector<int> dataShape, int index, bool verbose)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, StructuralModels::SetStructuralModel, data, dataShape, index, false, verbose);
}

LoopProjectFileResponse SetGeophysicalModel(std::string filename, std::vector<float> data, std::vector<int> dataShape, int index, bool verbose)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, GeophysicalModels::SetGeophysicalModel, data, dataShape, index, false, verbose);
}

LoopProjectFileResponse SetUncertaintyModel(std::string filename, std::vector<float> data, std::vector<int> dataShape, int index, bool verbose)
{
    LPF_OPEN_RUN_WITH_SHAPE(filename, UncertaintyModels::SetUncertaintyModel, data, dataShape, index, false, verbose);
}

} // namespace LoopProjectFile

