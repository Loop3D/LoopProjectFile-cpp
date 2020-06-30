#include <list>
#include <math.h>
#include "LoopDataCollection.h"

namespace LoopProjectFile {

bool DataCollection::CheckDataCollectionValid(netCDF::NcGroup* rootNode, bool verbose)
{
    bool valid = true;
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        if (verbose) std::cout << "  Data Collection Group Present" << std::endl;
    } else {
        if (verbose) std::cout << "No Data Collection Group Present" << std::endl;
    }
    return valid;
}

LoopProjectFileResponse DataCollection::CreateObservationGroup(netCDF::NcGroup* dataCollectionGroup, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    std::vector<int> foliationNameLength; foliationNameLength.push_back(LOOP_FOLIATION_NAME_LENGTH);
    std::vector<int> whatIsFoldedLength; whatIsFoldedLength.push_back(LOOP_FOLIATION_NAME_LENGTH);
    try {
        dataCollectionGroup->addGroup("Observations");
        netCDF::NcGroup observationGroup = dataCollectionGroup->getGroup("Observations");
        netCDF::NcDim faultIndex = observationGroup.addDim("faultIndex");
        netCDF::NcDim foldIndex = observationGroup.addDim("foldIndex");
        netCDF::NcDim foliationIndex = observationGroup.addDim("foliationIndex");
        netCDF::NcDim discontinuityIndex = observationGroup.addDim("discontinuityIndex");

        // Create fault observation compound type and variable
        netCDF::NcCompoundType faultObservationType = observationGroup.addCompoundType("faultObservation",sizeof(FaultObservation));
        faultObservationType.addMember("eventId",netCDF::ncInt,offsetof(FaultObservation, eventId));
        faultObservationType.addMember("easting",netCDF::ncDouble,offsetof(FaultObservation, easting));
        faultObservationType.addMember("northing",netCDF::ncDouble,offsetof(FaultObservation, northing));
        faultObservationType.addMember("altitude",netCDF::ncDouble,offsetof(FaultObservation, altitude));
        faultObservationType.addMember("dipdir",netCDF::ncDouble,offsetof(FaultObservation, dipdir));
        faultObservationType.addMember("dip",netCDF::ncDouble,offsetof(FaultObservation, dip));
        faultObservationType.addMember("dipPolarity",netCDF::ncDouble,offsetof(FaultObservation, dipPolarity));
        faultObservationType.addMember("val",netCDF::ncDouble,offsetof(FaultObservation, val));
        faultObservationType.addMember("displacement",netCDF::ncDouble,offsetof(FaultObservation, displacement));
        observationGroup.addVar("faultObservations",faultObservationType,faultIndex);

        // Create fold observation compound type and variable
        netCDF::NcCompoundType foldObservationType = observationGroup.addCompoundType("foldObservation",sizeof(FoldObservation));
        foldObservationType.addMember("eventId",netCDF::ncInt,offsetof(FoldObservation, eventId));
        foldObservationType.addMember("easting",netCDF::ncDouble,offsetof(FoldObservation, easting));
        foldObservationType.addMember("northing",netCDF::ncDouble,offsetof(FoldObservation, northing));
        foldObservationType.addMember("altitude",netCDF::ncDouble,offsetof(FoldObservation, altitude));
        foldObservationType.addMember("axisX",netCDF::ncDouble,offsetof(FoldObservation, axisX));
        foldObservationType.addMember("axisY",netCDF::ncDouble,offsetof(FoldObservation, axisY));
        foldObservationType.addMember("axisZ",netCDF::ncDouble,offsetof(FoldObservation, axisZ));
        foldObservationType.addMember("foliation",netCDF::ncChar,offsetof(FoldObservation, foliation),foliationNameLength);
        foldObservationType.addMember("whatIsFolded",netCDF::ncChar,offsetof(FoldObservation, whatIsFolded),whatIsFoldedLength);
        observationGroup.addVar("foldObservations",foldObservationType,foldIndex);

        // Create fold observation compound type and variable
        netCDF::NcCompoundType foliationObservationType = observationGroup.addCompoundType("foliationObservation",sizeof(FoliationObservation));
        foliationObservationType.addMember("eventId",netCDF::ncInt,offsetof(FoliationObservation, eventId));
        foliationObservationType.addMember("easting",netCDF::ncDouble,offsetof(FoliationObservation, easting));
        foliationObservationType.addMember("northing",netCDF::ncDouble,offsetof(FoliationObservation, northing));
        foliationObservationType.addMember("altitude",netCDF::ncDouble,offsetof(FoliationObservation, altitude));
        foliationObservationType.addMember("dipdir",netCDF::ncDouble,offsetof(FoliationObservation, dipdir));
        foliationObservationType.addMember("dir",netCDF::ncDouble,offsetof(FoliationObservation, dip));
        observationGroup.addVar("foliationObservations",foliationObservationType,foliationIndex);

        // Create discontinuity observation compound type and variable
        netCDF::NcCompoundType discontinuityObservationType = observationGroup.addCompoundType("discontinuityObservation",sizeof(DiscontinuityObservation));
        discontinuityObservationType.addMember("eventId",netCDF::ncInt,offsetof(DiscontinuityObservation, eventId));
        discontinuityObservationType.addMember("easting",netCDF::ncDouble,offsetof(DiscontinuityObservation, easting));
        discontinuityObservationType.addMember("northing",netCDF::ncDouble,offsetof(DiscontinuityObservation, northing));
        discontinuityObservationType.addMember("altitude",netCDF::ncDouble,offsetof(DiscontinuityObservation, altitude));
        discontinuityObservationType.addMember("dipdir",netCDF::ncDouble,offsetof(DiscontinuityObservation, dipdir));
        discontinuityObservationType.addMember("dir",netCDF::ncDouble,offsetof(DiscontinuityObservation, dip));
        observationGroup.addVar("discontinuityObservations",discontinuityObservationType,discontinuityIndex);
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what() << std::endl;
        resp = createErrorMsg(1,"Failed to create Data Collection Group and Types",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::GetFaultObservations(netCDF::NcGroup* rootNode, std::vector<FaultObservation>& observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") != dcGroups.end()) {
            netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
            netCDF::NcVar faultObs = observationGroup.getVar("faultObservations");
            for (size_t i=0;i<observationGroup.getDim("faultIndex").getSize();i++) {
                FaultObservation obs;
                std::vector<size_t> start; start.push_back(i);
                faultObs.getVar(start,&obs);
                observations.push_back(obs);
            }
        } else {
            resp = createErrorMsg(1,"No Observations Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::GetFoldObservations(netCDF::NcGroup* rootNode, std::vector<FoldObservation>& observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") != dcGroups.end()) {
            netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
            netCDF::NcVar foldObs = observationGroup.getVar("foldObservations");
            for (size_t i=0;i<observationGroup.getDim("foldIndex").getSize();i++) {
                FoldObservation obs;
                std::vector<size_t> start; start.push_back(i);
                foldObs.getVar(start,&obs);
                observations.push_back(obs);
            }
        } else {
            resp = createErrorMsg(1,"No Observations Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::GetFoliationObservations(netCDF::NcGroup* rootNode, std::vector<FoliationObservation>& observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") != dcGroups.end()) {
            netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
            netCDF::NcVar foliationObs = observationGroup.getVar("foliationObservations");
            for (size_t i=0;i<observationGroup.getDim("foliationIndex").getSize();i++) {
                FoliationObservation obs;
                std::vector<size_t> start; start.push_back(i);
                foliationObs.getVar(start,&obs);
                observations.push_back(obs);
            }
        } else {
            resp = createErrorMsg(1,"No Observations Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::GetDiscontinuityObservations(netCDF::NcGroup* rootNode, std::vector<DiscontinuityObservation>& observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") != dcGroups.end()) {
            netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
            netCDF::NcVar discontinuityObs = observationGroup.getVar("discontinuityObservations");
            for (size_t i=0;i<observationGroup.getDim("discontinuityIndex").getSize();i++) {
                DiscontinuityObservation obs;
                std::vector<size_t> start; start.push_back(i);
                discontinuityObs.getVar(start,&obs);
                observations.push_back(obs);
            }
        } else {
            resp = createErrorMsg(1,"No Observations Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::SetFaultObservations(netCDF::NcGroup* rootNode, std::vector<FaultObservation> observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("DataCollection") == groups.end()) {
            rootNode->addGroup("DataCollection");
        }
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") == dcGroups.end()) {
            resp = CreateObservationGroup(&dataCollectionGroup,verbose);
        }
        netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(observations.size());
        netCDF::NcVar faultObs = observationGroup.getVar("faultObservations");
        faultObs.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add fault data to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::SetFoldObservations(netCDF::NcGroup* rootNode, std::vector<FoldObservation> observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("DataCollection") == groups.end()) {
            rootNode->addGroup("DataCollection");
        }
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") == dcGroups.end()) {
            resp = CreateObservationGroup(&dataCollectionGroup);
        }
        netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(observations.size());
        netCDF::NcVar foldObs = observationGroup.getVar("foldObservations");
        foldObs.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add fold data to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::SetFoliationObservations(netCDF::NcGroup* rootNode, std::vector<FoliationObservation> observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("DataCollection") == groups.end()) {
            rootNode->addGroup("DataCollection");
        }
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") == dcGroups.end()) {
            resp = CreateObservationGroup(&dataCollectionGroup);
        }
        netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(observations.size());
        netCDF::NcVar foliationObs = observationGroup.getVar("foliationObservations");
        foliationObs.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add foliation data to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::SetDiscontinuityObservations(netCDF::NcGroup* rootNode, std::vector<DiscontinuityObservation> observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("DataCollection") == groups.end()) {
            rootNode->addGroup("DataCollection");
        }
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") == dcGroups.end()) {
            resp = CreateObservationGroup(&dataCollectionGroup);
        }
        netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(observations.size());
        netCDF::NcVar discontinuityObs = observationGroup.getVar("discontinuityObservations");
        discontinuityObs.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add discontinuity data to loop project file",verbose);
    }
    return resp;
}

} // namespace LoopProjectFile