#include "LoopProjectFile.h"

bool testLoopProjectFileFunctions(void);

int main (int argc, char** argv)
{
    return testLoopProjectFileFunctions();
}

bool testLoopProjectFileFunctions(void)
{
    int errors = 0;
    // Test the creation of a basic loop project file
    LoopProjectFileResponse resp = {0,""};
    std::string filename = "testLoopProjectFile.loop3d";
    resp = LoopProjectFile::CreateBasicFile(filename);
    std::cout << (resp.errorCode ? resp.errorMessage : "Project File Created!") << std::endl;
    errors += resp.errorCode;

    // Check the version of the project file
    LoopProjectFile::LoopVersion ver = LoopProjectFile::GetVersion(filename);
    std::cout << "Project File Version = " <<
        ver.majorVersion << "." << ver.minorVersion << "." << ver.subVersion << std::endl;

    // Add extents to the file
    LoopProjectFile::LoopExtents extents;
    extents.minLatitude = 1.0;
    extents.maxLatitude = 2.0;
    extents.minLongitude = 1.0;
    extents.maxLongitude = 2.0;
    extents.minDepth = 1000.0;
    extents.maxDepth = 2000.0;
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

    // Check that the extents are saved in the file
    LoopProjectFile::LoopExtents extents2;
    resp = LoopProjectFile::GetExtents(filename,extents2);
    errors += resp.errorCode;
    std::cout << "minLatitude = " << extents2.minLatitude << std::endl;
    std::cout << "maxLatitude = " << extents2.maxLatitude << std::endl;
    std::cout << "minLongitude = " << extents2.minLongitude << std::endl;
    std::cout << "maxLongitude = " << extents2.maxLongitude << std::endl;
    std::cout << "minEasting = " << extents2.minEasting << std::endl;
    std::cout << "maxEasting = " << extents2.maxEasting << std::endl;
    std::cout << "minNorthing = " << extents2.minNorthing << std::endl;
    std::cout << "maxNorthing = " << extents2.maxNorthing << std::endl;
    std::cout << "minDepth = " << extents2.minDepth << std::endl;
    std::cout << "maxDepth = " << extents2.maxDepth << std::endl;
    std::cout << "utmZone = " << extents2.utmZone << std::endl;
    std::cout << "utmNorthSouth = " << extents2.utmNorthSouth << std::endl;
    std::cout << "workingFormat = " << extents2.workingFormat << std::endl;
    std::cout << "spacingX = " << extents2.spacingX << std::endl;
    std::cout << "spacingY = " << extents2.spacingY << std::endl;
    std::cout << "spacingZ = " << extents2.spacingZ << std::endl;
    std::cout << "extents error state = " << extents2.errored << std::endl;

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

    // Check that those fault observations are in the file
    faultObservations.clear();
    resp = LoopProjectFile::GetFaultObservations(filename,faultObservations,true);
    errors += resp.errorCode;
    for (auto i=0; i<faultObservations.size(); i++) {
        std::cout << "Fault Obs northing = " << faultObservations[i].northing << std::endl;
    }

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
    errors += resp.errorCode;

    // And check those fold observations
    foldObservations.clear();
    resp = LoopProjectFile::GetFoldObservations(filename,foldObservations,true);
    errors += resp.errorCode;
    for (auto i=0; i<foldObservations.size(); i++) {
        std::cout << "Fold Obs northing = " << foldObservations[i].northing << std::endl;
    }

    // Create fault events this time
    std::vector<LoopProjectFile::FaultEvent> faultEvents;
    for (auto i=0; i<5; i++) {
        LoopProjectFile::FaultEvent faultEvent;
        faultEvent.eventId = i;
        faultEvent.minAge = i*0.5;
        faultEvent.maxAge = i*0.5 + 0.5;
        std::string name = "Testing ";
        strncpy(faultEvent.name,name.c_str(),LOOP_NAME_LENGTH);
        faultEvents.push_back(faultEvent);
    }
    for (auto it=faultEvents.begin(); it!=faultEvents.end(); it++) {
        std::cout << "Initial Fault Event name = " << it->name << std::endl;
    }
    resp = LoopProjectFile::SetFaultEvents(filename,faultEvents,true);
    errors += resp.errorCode;

    // Check there are there
    faultEvents.clear();
    resp = LoopProjectFile::GetFaultEvents(filename,faultEvents,true);
    errors += resp.errorCode;
    for (auto it=faultEvents.begin(); it!=faultEvents.end(); it++) {
        std::cout << "Fault Event name = " << it->name << std::endl;
    }

    // Create data shape from extents above and some fake structural data
    // and add it to the project file
    std::vector<int> dataShape;
    int shapeX = (extents.maxEasting - extents.minEasting) / extents.spacingX + 1;
    int shapeY = (extents.maxNorthing - extents.minNorthing) / extents.spacingY + 1;
    int shapeZ = (extents.maxDepth - extents.minDepth) / extents.spacingZ + 1;
    dataShape.push_back(shapeX);
    dataShape.push_back(shapeY);
    dataShape.push_back(shapeZ);
    std::vector<float> data(dataShape[0]*dataShape[1]*dataShape[2]);
    for (auto i=0; i<dataShape[0]; i++)
        for (auto j=0; j<dataShape[1]; j++)
            for (auto k=0; k<dataShape[2]; k++)
                data[i*dataShape[1]*dataShape[2] + j*dataShape[2] + k] = i+j+k;
    resp = LoopProjectFile::SetStructuralModel(filename,data,dataShape,0,true);
    if (resp.errorCode) std::cout << resp.errorMessage << std::endl;
    errors += resp.errorCode;

    // Check the structural model data and data shape match insertion
    data.clear();
    dataShape.clear();
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

    // Run the file check valid function 
    if (LoopProjectFile::CheckFileValid(filename,true)) {
        std::cout << "Loop Project File " << filename << " checked out as valid" << std::endl;
    } else {
        std::cout << "Loop Project File " << filename << " checked out as NOT valid" << std::endl;
    }

    return errors;
}
