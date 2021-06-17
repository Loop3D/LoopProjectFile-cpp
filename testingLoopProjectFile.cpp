#include "LoopProjectFile.h"
#include <sys/stat.h>

int testLoopProjectFileCreateFunctions(std::string filename);
int testLoopProjectFileSetFunctions(std::string filename);
int testLoopProjectFileGetFunctions(std::string filename);

int main (int argc, char** argv)
{
    std::string filename = "testLoopProjectFile.loop3d";
    int errors = 0;
    struct stat buffer;
    if (stat (filename.c_str(), &buffer) != 0) {
        std::cout << "File " << filename << " does not exist, creating!" << std::endl;
        errors += testLoopProjectFileCreateFunctions(filename);
        errors += testLoopProjectFileSetFunctions(filename);
    } else {
        std::cout << "File " << filename << " exists reading data!" << std::endl;
    }

    errors += testLoopProjectFileGetFunctions(filename);

    // Run the file check valid function 
    if (LoopProjectFile::CheckFileValid(filename,true)) {
        std::cout << "Loop Project File " << filename << " checked out as valid" << std::endl;
    } else {
        std::cout << "Loop Project File " << filename << " checked out as NOT valid" << std::endl;
    }

    return errors;
}

int testLoopProjectFileCreateFunctions(std::string filename)
{
    int errors = 0;
    // Test the creation of a basic loop project file
    LoopProjectFileResponse resp = {0,""};
    resp = LoopProjectFile::CreateBasicFile(filename);
    std::cout << (resp.errorCode ? resp.errorMessage : "Project File Created!") << std::endl;
    errors += resp.errorCode;

    // Check the version of the project file
    LoopProjectFile::LoopVersion ver = LoopProjectFile::GetVersion(filename);
    std::cout << "Project File Version = " <<
        ver.majorVersion << "." << ver.minorVersion << "." << ver.subVersion << std::endl;

    return errors;
}

int testLoopProjectFileSetFunctions(std::string filename)
{
    int errors = 0;
    LoopProjectFileResponse resp = {0,""};

    // Add extents to the file
    LoopProjectFile::LoopExtents extents;
    extents.minLatitude = 1.0;
    extents.maxLatitude = 2.0;
    extents.minLongitude = 1.0;
    extents.maxLongitude = 2.0;
    extents.topDepth = -1000.0;
    extents.bottomDepth = -2000.0;
    extents.minEasting = 1000.0;
    extents.maxEasting = 2500.0;
    extents.minNorthing = 1000.0;
    extents.maxNorthing = 2000.0;
    extents.utmZone = 20;
    extents.utmNorthSouth = 1;
    extents.workingFormat = 0;
    extents.spacingX = 10;
    extents.spacingY = 10;
    extents.spacingZ = 10;
    extents.errored = false;
    resp = LoopProjectFile::SetExtents(filename,extents);
    if (resp.errorCode) std::cout << resp.errorMessage << std::endl;
    errors += resp.errorCode;

    // Create some fault observations and save them to the file
    std::vector<LoopProjectFile::FaultObservation> faultObservations;
    for (auto i=0; i<10; i++) {
        LoopProjectFile::FaultObservation faultObs;
        faultObs.northing = i;
        faultObs.easting = i;
        faultObs.altitude = i;
        faultObs.dipdir = i;
        faultObs.dip = i;
        faultObservations.push_back(faultObs);
    }
    resp = LoopProjectFile::SetFaultObservations(filename,faultObservations,true);
    errors += resp.errorCode;

    // Repeat for fold observations
    std::vector<LoopProjectFile::FoldObservation> foldObservations;
    for (auto i=0; i<5; i++) {
        LoopProjectFile::FoldObservation foldObs;
        foldObs.northing = i;
        foldObs.easting = i;
        foldObs.altitude = i;
        foldObservations.push_back(foldObs);
    }
    resp = LoopProjectFile::SetFoldObservations(filename,foldObservations,true);
    if (resp.errorCode) std::cout << resp.errorMessage << std::endl;
    errors += resp.errorCode;

    // Create fault events this time
    std::vector<LoopProjectFile::FaultEvent> faultEvents;
    for (auto i=0; i<5; i++) {
        LoopProjectFile::FaultEvent faultEvent;
        faultEvent.eventId = i;
        faultEvent.minAge = i*0.5;
        faultEvent.maxAge = i*0.5 + 0.5;
        std::string name = "Testing ";
        strncpy_s(faultEvent.name,name.c_str(),LOOP_NAME_LENGTH);
        faultEvent.centreEasting = 1;
        faultEvent.centreNorthing = 1;
        faultEvent.centreAltitude = 1;
        faultEvent.avgSlipDirEasting = 2;
        faultEvent.avgSlipDirNorthing = 2;
        faultEvent.avgSlipDirAltitude = 2;
        faultEvent.avgNormalEasting = 3;
        faultEvent.avgNormalNorthing = 3;
        faultEvent.avgNormalAltitude = 3;
        faultEvents.push_back(faultEvent);
    }
    for (auto it=faultEvents.begin(); it!=faultEvents.end(); it++) {
        std::cout << "Initial Fault Event (ID: " << it->eventId << ") name = " << it->name << std::endl;
    }
    resp = LoopProjectFile::SetFaultEvents(filename,faultEvents,true);
    if (resp.errorCode) std::cout << resp.errorMessage << std::endl;
    errors += resp.errorCode;

    // Create data shape from extents above and some fake structural data
    // and add it to the project file
    std::vector<int> dataShape;
    int shapeX = (int)((extents.maxEasting - extents.minEasting) / extents.spacingX + 1);
    int shapeY = (int)((extents.maxNorthing - extents.minNorthing) / extents.spacingY + 1);
    int shapeZ = (int)((extents.topDepth - extents.bottomDepth) / extents.spacingZ + 1);
    dataShape.push_back(shapeX);
    dataShape.push_back(shapeY);
    dataShape.push_back(shapeZ);
    std::vector<float> data(dataShape[0]*dataShape[1]*dataShape[2]);
    for (auto i=0; i<dataShape[0]; i++)
        for (auto j=0; j<dataShape[1]; j++)
            for (auto k=0; k<dataShape[2]; k++)
                data[i*dataShape[1]*dataShape[2] + j*dataShape[2] + k] = (float)(i+j+k);
    resp = LoopProjectFile::SetStructuralModel(filename,data,dataShape,0,true);
    if (resp.errorCode) std::cout << resp.errorMessage << std::endl;
    errors += resp.errorCode;

    return errors;
}
int testLoopProjectFileGetFunctions(std::string filename)
{
    int errors = 0;
    LoopProjectFileResponse resp = {0,""};

    // Check that the extents are saved in the file
    LoopProjectFile::LoopExtents extents;
    resp = LoopProjectFile::GetExtents(filename,extents);
    errors += resp.errorCode;
    std::cout << "minLatitude = " << extents.minLatitude << std::endl;
    std::cout << "maxLatitude = " << extents.maxLatitude << std::endl;
    std::cout << "minLongitude = " << extents.minLongitude << std::endl;
    std::cout << "maxLongitude = " << extents.maxLongitude << std::endl;
    std::cout << "minEasting = " << extents.minEasting << std::endl;
    std::cout << "maxEasting = " << extents.maxEasting << std::endl;
    std::cout << "minNorthing = " << extents.minNorthing << std::endl;
    std::cout << "maxNorthing = " << extents.maxNorthing << std::endl;
    std::cout << "topDepth = " << extents.topDepth << std::endl;
    std::cout << "bottomDepth = " << extents.bottomDepth << std::endl;
    std::cout << "utmZone = " << extents.utmZone << std::endl;
    std::cout << "utmNorthSouth = " << extents.utmNorthSouth << std::endl;
    std::cout << "workingFormat = " << extents.workingFormat << std::endl;
    std::cout << "spacingX = " << extents.spacingX << std::endl;
    std::cout << "spacingY = " << extents.spacingY << std::endl;
    std::cout << "spacingZ = " << extents.spacingZ << std::endl;
    std::cout << "extents error state = " << extents.errored << std::endl;

    // Check that those fault observations are in the file
    std::vector<LoopProjectFile::FaultObservation> faultObservations;
    resp = LoopProjectFile::GetFaultObservations(filename,faultObservations,true);
    if (resp.errorCode) std::cout << resp.errorMessage << std::endl;
    errors += resp.errorCode;
    for (auto i=0; i<faultObservations.size(); i++) {
        std::cout << "Fault Obs for event " << faultObservations[i].eventId << ": " <<
        "(" << faultObservations[i].easting << "," << faultObservations[i].northing << "," << faultObservations[i].altitude << ") "
        << (int)faultObservations[i].type << " " << faultObservations[i].dipdir << " " << faultObservations[i].dip << " "
        << faultObservations[i].dipPolarity << " " << faultObservations[i].val << " " << faultObservations[i].displacement << " "
        << std::endl;
    }

    // And check those fold observations
    std::vector<LoopProjectFile::FoldObservation> foldObservations;
    resp = LoopProjectFile::GetFoldObservations(filename,foldObservations,true);
    if (resp.errorCode) std::cout << resp.errorMessage << std::endl;
    errors += resp.errorCode;
    for (auto i=0; i<foldObservations.size(); i++) {
        std::cout << "Fold Obs for event " << foldObservations[i].eventId << ": " <<
        "(" << foldObservations[i].easting << "," << foldObservations[i].northing << "," << foldObservations[i].altitude << ") "
        << "(" << foldObservations[i].axisX << "," << foldObservations[i].axisY << "," << foldObservations[i].axisZ << ") "
        << (int)foldObservations[i].type << " " << foldObservations[i].foliation << " " << foldObservations[i].whatIsFolded << " "
        << std::endl;
    }

    // Check fault events are there
    std::vector<LoopProjectFile::FaultEvent> faultEvents;
    resp = LoopProjectFile::GetFaultEvents(filename,faultEvents,true);
    if (resp.errorCode) std::cout << resp.errorMessage << std::endl;
    errors += resp.errorCode;
    for (auto it=faultEvents.begin(); it!=faultEvents.end(); it++) {
        std::cout << "Fault Event (ID: " << it->eventId << ") = " << it->minAge << " " << it->maxAge << " " 
        << it->name << " " << (int)it->enabled << " " << it->rank << " " << (int)it->type << " " << it->avgDisplacement
        << std::endl;
    }

    // Check the structural model data and data shape match insertion
    std::vector<int> dataShape;
    int shapeX = (int)((extents.maxEasting - extents.minEasting) / extents.spacingX + 1);
    int shapeY = (int)((extents.maxNorthing - extents.minNorthing) / extents.spacingY + 1);
    int shapeZ = (int)((extents.topDepth - extents.bottomDepth) / extents.spacingZ + 1);
    dataShape.push_back(shapeX);
    dataShape.push_back(shapeY);
    dataShape.push_back(shapeZ);
    std::vector<float> data(dataShape[0]*dataShape[1]*dataShape[2]);
    resp = LoopProjectFile::GetStructuralModel(filename,data,dataShape,0,true);
    errors += resp.errorCode;
    if (dataShape.size() != 3 || dataShape[0] != shapeX || dataShape[1] != shapeY || dataShape[2] != shapeZ) {
        std::cout << "Data Shape returned does not match original insertion" << std::endl;
        bool broken = false;
        for (auto i=0; i<dataShape[0] && !broken; i++)
            for (auto j=0; j<dataShape[1] && !broken; j++)
                for (auto k=0; k<dataShape[2] && !broken; k++)
                    if (data[i*dataShape[1]*dataShape[2] + j*dataShape[2] + k] != i+j+k) {
                        std::cout << "Values did not return the same" << std::endl;
                        broken = true;
                    }
    }

    return errors;
}
