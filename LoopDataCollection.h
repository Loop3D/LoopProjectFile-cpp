#ifndef __LOOPDATACOLLECTION_H
#define __LOOPDATACOLLECTION_H

#include <list>
#include <math.h>
#include "LoopProjectFileUtils.h"

#define LOOP_FOLIATION_NAME_LENGTH 30
#define LOOP_WHAT_IS_FOLDED_NAME_LENGTH 30


namespace LoopProjectFile {

/*! \brief A structure describing a single fault observation */
struct FaultObservation {
    int eventId; /*!< The event identifier of the event that this observation belongs to */
    /*! @{ The location of this observation in UTM coordinates (metres) */
    double easting;
    double northing;
    double altitude;
    /*!@}*/
    double dipdir; /*!< The compass direction of the observed fault dip (degrees) */
    double dip; /*!< The angle of the observed fault dip 0=horizontal 90=vertical down (degrees) */
    double polarity; /*!< The polarity of the dip 0=normal 1=inverted */
    double val; /*!< The calculated depth on the stratographic column of the observation (metres) */
    double displacement; /*!< The displacement of the fault at this observation */
    /*! Contructor. Zeros all variables */
    FaultObservation() {
        eventId = 0;
        easting = 0;
        northing = 0;
        altitude = 0;
        dipdir = 0;
        dip = 0;
        polarity = 0;
        val = 0;
        displacement = 0;
    }
};

/*! \brief A structure describing a single fold observation */
struct FoldObservation {
    int eventId; /*!< The event identifier of the event that this observation belongs to */
    /*! @{ The location of this observation in UTM coordinates (metres) */
    double easting;
    double northing;
    double altitude;
    /*!@}*/
    /*! @{ The 3D vector of the observed fold axis in XYZ (arbitrary units) */
    double axisX;
    double axisY;
    double axisZ;
    /*!@}*/
    char foliation[LOOP_FOLIATION_NAME_LENGTH]; /*!< The name of the most recent proceeding foliation event that is folded */
    char whatIsFolded[LOOP_WHAT_IS_FOLDED_NAME_LENGTH]; /*!< The name of the most recent proceeding fold event (if any) that is folded */
    /*! Contructor. Zeros all variables */ 
    FoldObservation() {
        eventId = 0;
        easting = 0;
        northing = 0;
        altitude = 0;
        axisX = 0;
        axisY = 0;
        axisZ = 0;
        for (auto i=0;i<LOOP_FOLIATION_NAME_LENGTH;i++) foliation[i] = 0;
        for (auto i=0;i<LOOP_WHAT_IS_FOLDED_NAME_LENGTH;i++) whatIsFolded[i] = 0;
        whatIsFolded[0] = 0;
    }
};

/*! \brief A structure describing a single foliation observation */
struct FoliationObservation {
    int eventId; /*!< The event identifier of the event that this observation belongs to */
    /*! @{ The location of this observation in UTM coordinates (metres) */
    double easting;
    double northing;
    double altitude;
    /*!@}*/
    double dipdir; /*!< The compass direction of the observed foliation dip (degrees) */
    double dip; /*!< The angle of the observed foliation dip 0=horizontal 90=vertical down (degrees) */
    /*! Contructor. Zeros all variables */
    FoliationObservation() {
        eventId = 0;
        easting = 0;
        northing = 0;
        altitude = 0;
        dipdir = 0;
        dip = 0;
    }
};

/*! \brief A structure describing a single discontinuity observation */
struct DiscontinuityObservation {
    int eventId; /*!< The event identifier of the event that this observation belongs to */
    /*! @{ The location of this observation in UTM coordinates (metres) */
    double easting;
    double northing;
    double altitude;
    /*!@}*/
    double dipdir; /*!< The compass direction of the observed discontinuity dip (degrees) */
    double dip; /*!< The angle of the observed discontinuity dip 0=horizontal 90=vertical down (degrees) */
    /*! Contructor. Zeros all variables */
    DiscontinuityObservation() {
        eventId = 0;
        easting = 0;
        northing = 0;
        altitude = 0;
        dipdir = 0;
        dip = 0;
    }
};

namespace DataCollection {

/*!
 * \brief Checks the structure of the given netCDF root node for the appropriate
 * data collection structure
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return  A flag indicating whether the netCDF file is valid for data collection data
 */
bool CheckDataCollectionValid(netCDF::NcGroup* rootNode, bool verbose=false)
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

/*!
 * \brief Creates the data collection structure in the given netCDF root node
 *
 * \param dataCollectionGroup - a pointer to the netCDF Group node "DataCollection"
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of structure creation with an error message if it failed
 */
LoopProjectFileResponse CreateObservationGroup(netCDF::NcGroup* dataCollectionGroup, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    std::vector<int> foliationNameLength; foliationNameLength.push_back(LOOP_FOLIATION_NAME_LENGTH);
    std::vector<int> whatIsFoldedLength; whatIsFoldedLength.push_back(LOOP_FOLIATION_NAME_LENGTH);
    try {
        dataCollectionGroup->addGroup("Observations");
        netCDF::NcGroup observationGroup = dataCollectionGroup->getGroup("Observations");
        netCDF::NcDim faultIndex = observationGroup.addDim("FaultIndex");
        netCDF::NcDim foldIndex = observationGroup.addDim("FoldIndex");
        netCDF::NcDim foliationIndex = observationGroup.addDim("FoliationIndex");
        netCDF::NcDim discontinuityIndex = observationGroup.addDim("DiscontinuityIndex");

        // Create fault observation compound type and variable
        netCDF::NcCompoundType faultObservationType = observationGroup.addCompoundType("FaultObservation",sizeof(FaultObservation));
        faultObservationType.addMember("eventId",netCDF::ncInt,offsetof(FaultObservation, eventId));
        faultObservationType.addMember("easting",netCDF::ncDouble,offsetof(FaultObservation, easting));
        faultObservationType.addMember("northing",netCDF::ncDouble,offsetof(FaultObservation, northing));
        faultObservationType.addMember("altitude",netCDF::ncDouble,offsetof(FaultObservation, altitude));
        faultObservationType.addMember("dipdir",netCDF::ncDouble,offsetof(FaultObservation, dipdir));
        faultObservationType.addMember("dip",netCDF::ncDouble,offsetof(FaultObservation, dip));
        faultObservationType.addMember("polarity",netCDF::ncDouble,offsetof(FaultObservation, polarity));
        faultObservationType.addMember("val",netCDF::ncDouble,offsetof(FaultObservation, val));
        faultObservationType.addMember("displacement",netCDF::ncDouble,offsetof(FaultObservation, displacement));
        observationGroup.addVar("FaultObservations",faultObservationType,faultIndex);

        // Create fold observation compound type and variable
        netCDF::NcCompoundType foldObservationType = observationGroup.addCompoundType("FoldObservation",sizeof(FoldObservation));
        foldObservationType.addMember("eventId",netCDF::ncInt,offsetof(FoldObservation, eventId));
        foldObservationType.addMember("easting",netCDF::ncDouble,offsetof(FoldObservation, easting));
        foldObservationType.addMember("northing",netCDF::ncDouble,offsetof(FoldObservation, northing));
        foldObservationType.addMember("altitude",netCDF::ncDouble,offsetof(FoldObservation, altitude));
        foldObservationType.addMember("axisX",netCDF::ncDouble,offsetof(FoldObservation, axisX));
        foldObservationType.addMember("axisY",netCDF::ncDouble,offsetof(FoldObservation, axisY));
        foldObservationType.addMember("axisZ",netCDF::ncDouble,offsetof(FoldObservation, axisZ));
        foldObservationType.addMember("foliation",netCDF::ncChar,offsetof(FoldObservation, foliation),foliationNameLength);
        foldObservationType.addMember("whatIsFolded",netCDF::ncChar,offsetof(FoldObservation, whatIsFolded),whatIsFoldedLength);
        observationGroup.addVar("FoldObservations",foldObservationType,foldIndex);

        // Create fold observation compound type and variable
        netCDF::NcCompoundType foliationObservationType = observationGroup.addCompoundType("FoliationObservation",sizeof(FoliationObservation));
        foliationObservationType.addMember("eventId",netCDF::ncInt,offsetof(FoliationObservation, eventId));
        foliationObservationType.addMember("easting",netCDF::ncDouble,offsetof(FoliationObservation, easting));
        foliationObservationType.addMember("northing",netCDF::ncDouble,offsetof(FoliationObservation, northing));
        foliationObservationType.addMember("altitude",netCDF::ncDouble,offsetof(FoliationObservation, altitude));
        foliationObservationType.addMember("dipdir",netCDF::ncDouble,offsetof(FoliationObservation, dipdir));
        foliationObservationType.addMember("dir",netCDF::ncDouble,offsetof(FoliationObservation, dip));
        observationGroup.addVar("FoliationObservations",foliationObservationType,foliationIndex);

        // Create discontinuity observation compound type and variable
        netCDF::NcCompoundType discontinuityObservationType = observationGroup.addCompoundType("DiscontinuityObservation",sizeof(DiscontinuityObservation));
        discontinuityObservationType.addMember("eventId",netCDF::ncInt,offsetof(DiscontinuityObservation, eventId));
        discontinuityObservationType.addMember("easting",netCDF::ncDouble,offsetof(DiscontinuityObservation, easting));
        discontinuityObservationType.addMember("northing",netCDF::ncDouble,offsetof(DiscontinuityObservation, northing));
        discontinuityObservationType.addMember("altitude",netCDF::ncDouble,offsetof(DiscontinuityObservation, altitude));
        discontinuityObservationType.addMember("dipdir",netCDF::ncDouble,offsetof(DiscontinuityObservation, dipdir));
        discontinuityObservationType.addMember("dir",netCDF::ncDouble,offsetof(DiscontinuityObservation, dip));
        observationGroup.addVar("DiscontinuityObservations",discontinuityObservationType,discontinuityIndex);
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what() << std::endl;
        resp = createErrorMsg(1,"Failed to create Data Collection Group and Types",verbose);
    }
    return resp;
}

/*!
 * \brief Retrieves fault observation data from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - a reference to where the observation data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFaultObservations(netCDF::NcGroup* rootNode, std::vector<FaultObservation>& observations, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") != dcGroups.end()) {
            netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
            netCDF::NcVar faultObs = observationGroup.getVar("FaultObservations");
            for (size_t i=0;i<observationGroup.getDim("FaultIndex").getSize();i++) {
                FaultObservation obs;
                std::vector<size_t> start; start.push_back(i);
                faultObs.getVar(start,&obs);
                observations.push_back(obs);
            }
        } else {
            resp = createErrorMsg(1,"No Observations Group Node Present");
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present");
    }
    return resp;
}

/*!
 * \brief Retrieves fold observation data from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - a reference to where the observation data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFoldObservations(netCDF::NcGroup* rootNode, std::vector<FoldObservation>& observations, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") != dcGroups.end()) {
            netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
            netCDF::NcVar foldObs = observationGroup.getVar("FoldObservations");
            for (size_t i=0;i<observationGroup.getDim("FoldIndex").getSize();i++) {
                FoldObservation obs;
                std::vector<size_t> start; start.push_back(i);
                foldObs.getVar(start,&obs);
                observations.push_back(obs);
            }
        } else {
            resp = createErrorMsg(1,"No Observations Group Node Present");
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present");
    }
    return resp;
}

/*!
 * \brief Retrieves foliation observation data from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - a reference to where the observation data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFoliationObservations(netCDF::NcGroup* rootNode, std::vector<FoliationObservation>& observations, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") != dcGroups.end()) {
            netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
            netCDF::NcVar foliationObs = observationGroup.getVar("FoliationObservations");
            for (size_t i=0;i<observationGroup.getDim("FoliationIndex").getSize();i++) {
                FoliationObservation obs;
                std::vector<size_t> start; start.push_back(i);
                foliationObs.getVar(start,&obs);
                observations.push_back(obs);
            }
        } else {
            resp = createErrorMsg(1,"No Observations Group Node Present");
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present");
    }
    return resp;
}

/*!
 * \brief Retrieves discontinuity observation data from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - a reference to where the observation data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetDiscontinuityObservations(netCDF::NcGroup* rootNode, std::vector<DiscontinuityObservation>& observations, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") != dcGroups.end()) {
            netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
            netCDF::NcVar discontinuityObs = observationGroup.getVar("DiscontinuityObservations");
            for (size_t i=0;i<observationGroup.getDim("discontinuityIndex").getSize();i++) {
                DiscontinuityObservation obs;
                std::vector<size_t> start; start.push_back(i);
                discontinuityObs.getVar(start,&obs);
                observations.push_back(obs);
            }
        } else {
            resp = createErrorMsg(1,"No Observations Group Node Present");
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present");
    }
    return resp;
}

/*!
 * \brief Sets fault observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetFaultObservations(netCDF::NcGroup* rootNode, std::vector<FaultObservation> observations, bool verbose=false)
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
        netCDF::NcVar faultObs = observationGroup.getVar("FaultObservations");
        faultObs.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add fault data to loop project file");
    }
    return resp;
}

/*!
 * \brief Sets fold observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoldObservations(netCDF::NcGroup* rootNode, std::vector<FoldObservation> observations, bool verbose=false)
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
        netCDF::NcVar foldObs = observationGroup.getVar("FoldObservations");
        foldObs.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add fold data to loop project file");
    }
    return resp;
}

/*!
 * \brief Sets foliation observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoliationObservations(netCDF::NcGroup* rootNode, std::vector<FoliationObservation> observations, bool verbose=false)
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
        netCDF::NcVar foliationObs = observationGroup.getVar("FoliationObservations");
        foliationObs.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add foliation data to loop project file");
    }
    return resp;
}

/*!
 * \brief Sets discontinuity observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetDiscontinuityObservations(netCDF::NcGroup* rootNode, std::vector<DiscontinuityObservation> observations, bool verbose=false)
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
        netCDF::NcVar discontinuityObs = observationGroup.getVar("DiscontinuityObservations");
        discontinuityObs.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add discontinuity data to loop project file");
    }
    return resp;
}

} // namespace DataCollection
} // namespace LoopProjectFile

#endif