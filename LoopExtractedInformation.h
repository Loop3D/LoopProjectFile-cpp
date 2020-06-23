#ifndef __LOOPEXTRACTEDINFORMATION_H
#define __LOOPEXTRACTEDINFORMATION_H

#include "LoopProjectFileUtils.h"
#define LOOP_NAME_LENGTH 30

namespace LoopProjectFile {

/*! \brief A structure describing a fault event */
struct FaultEvent {
    int eventId; /*!< A unique event identifier (unique to all events types) */
    /*!@{ Limits on the age of the event (arbitrary units) */
    double minAge;
    double maxAge;
    /*!@}*/
    double avgDisplacement; /*!< The calculated average displacement across the fault (metres) */
    char enabled; /*!< A flag to enable/disable this event from modelling calculations */
    char name[LOOP_NAME_LENGTH]; /*!< The name of this event */
    /*! Constructor. Zeros all values */
    FaultEvent() {
        eventId = 0;
        minAge = 0;
        maxAge = 0;
        avgDisplacement = 0;
        enabled = 0;
        for (auto i=0;i<LOOP_NAME_LENGTH;i++) name[i] = 0;
    }
};

/*! \brief A structure describing a fold event */
struct FoldEvent {
    int eventId; /*!<A unique event identifier (unique to all events types) */
    /*!@{ Limits on the age of the event (arbitrary units) */
    double minAge;
    double maxAge;
    /*!@}*/
    char periodic; /*!< A flag indicating whether the fold is repeated */
    double wavelength; /*!< The wavelength of the fold (metres) */
    double amplitude; /*!< The amplitude of the fold (metres) */
    char asymmetry; /*!< A flag indicating whether the fold is asymmetrical */
    double asymmetryShift; /*!< The ratio of the asymmetry */
    double secondaryWavelength; /*!< The wavelength of the parasitic fold (metres) */
    double secondaryAmplitude; /*!< The amplitude of the parasitic fold (metres) */
    char enabled; /*!< A flag to enable/disable this event from modelling calculations */
    char name[LOOP_NAME_LENGTH]; /*!< The name of this event */
    /*! Constructor. Zeros all values */
    FoldEvent() {
        eventId = 0;
        minAge = 0;
        maxAge = 0;
        periodic = 0;
        wavelength = 0;
        amplitude = 0;
        asymmetry = 0;
        asymmetryShift = 0;
        secondaryWavelength = 0;
        secondaryAmplitude = 0;
        enabled = 0;
        for (auto i=0;i<LOOP_NAME_LENGTH;i++) name[i] = 0;
    }
};

/*! \brief A structure describing a foliation event */
struct FoliationEvent {
    int eventId; /*!< A unique event identifier (unique to all events types) */
    /*!@{ Limits on the age of the event (arbitrary units) */
    double minAge;
    double maxAge;
    /*!@}*/
    /*!@{ The limits of the foliation event in depth on the stratographic log */
    double lowerScalarValue;
    double upperScalarValue;
    /*!@}*/
    char enabled; /*!< A flag to enable/disable this event from modelling calculations */
    char name[LOOP_NAME_LENGTH]; /*!< The name of this event */
    /*! Constructor. Zeros all values */
    FoliationEvent() {
        eventId = 0;
        minAge = 0;
        maxAge = 0;
        lowerScalarValue = 0;
        upperScalarValue = 0;
        enabled = 0;
        for (auto i=0;i<LOOP_NAME_LENGTH;i++) name[i] = 0;
    }
};

/*! \brief A structure describing a discontinuity event */
struct DiscontinuityEvent {
    int eventId; /*!<A unique event identifier (unique to all events types) */
    /*!@{ Limits on the age of the event (arbitrary units) */
    double minAge;
    double maxAge;
    /*!@}*/
    double scalarValue; /*!< The depth of the discontinuity event on the stratographic log */
    char enabled; /*!< A flag to enable/disable this event from modelling calculations */
    char name[LOOP_NAME_LENGTH]; /*!< The name of this event */
    /*! Constructor. Zeros all values */
    DiscontinuityEvent() {
        eventId = 0;
        minAge = 0;
        maxAge = 0;
        scalarValue = 0;
        enabled = 0;
        for (auto i=0;i<LOOP_NAME_LENGTH;i++) name[i] = 0;
    }
};

namespace ExtractedInformation {

/*! 
 * \brief Checks the structure of the given netCDF root node for the appropriate
 * extracted information structure
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return  A flag indicating whether the netCDF file is valid for extracted information data
 */
bool CheckExtractedInformationValid(netCDF::NcGroup* rootNode, bool verbose=false)
{
    bool valid = true;
    auto groups = rootNode->getGroups();
    if (groups.find("ExtractedInformation") != groups.end()) {
        if (verbose) std::cout << "  Extracted Information Group Present" << std::endl;
    } else {
        if (verbose) std::cout << "No Extracted Information Group Present" << std::endl;
    }
    return valid;
}

/*!
 * \brief Creates the extracted information structure in the given netCDF root node
 *
 * \param extractedInformationGroup - a pointer to the netCDF Group node "ExtractedInformation"
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of structure creation with an error message if it failed
 */
LoopProjectFileResponse CreateExtractedInformationGroup(netCDF::NcGroup* extractedInformationGroup, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    std::vector<int> length; length.push_back(LOOP_NAME_LENGTH);
    try {
        extractedInformationGroup->addGroup("EventLog");
        netCDF::NcGroup eventLogGroup = extractedInformationGroup->getGroup("EventLog");
        netCDF::NcDim faultEventIndex = eventLogGroup.addDim("FaultEventIndex");
        netCDF::NcDim foldEventIndex = eventLogGroup.addDim("FoldEventIndex");
        netCDF::NcDim foliationEventIndex = eventLogGroup.addDim("FoliationEventIndex");
        netCDF::NcDim discontinuityEventIndex = eventLogGroup.addDim("DiscontinuityEventIndex");

        // Create fault observation compound type and variable
        netCDF::NcCompoundType faultEventType = eventLogGroup.addCompoundType("FaultEvent",sizeof(FaultEvent));
        faultEventType.addMember("eventId",netCDF::ncInt,offsetof(FaultEvent, eventId));
        faultEventType.addMember("minAge",netCDF::ncDouble,offsetof(FaultEvent, minAge));
        faultEventType.addMember("maxAge",netCDF::ncDouble,offsetof(FaultEvent, maxAge));
        faultEventType.addMember("avgDisplacement",netCDF::ncDouble,offsetof(FaultEvent, avgDisplacement));
        faultEventType.addMember("enabled",netCDF::ncChar,offsetof(FaultEvent, enabled));
        faultEventType.addMember("name",netCDF::ncChar,offsetof(FaultEvent, name),length);
        eventLogGroup.addVar("FaultEvents",faultEventType,faultEventIndex);

        // Create fold observation compound type and variable
        netCDF::NcCompoundType foldEventType = eventLogGroup.addCompoundType("FoldEvent",sizeof(FoldEvent));
        foldEventType.addMember("eventId",netCDF::ncInt,offsetof(FoldEvent, eventId));
        foldEventType.addMember("minAge",netCDF::ncDouble,offsetof(FoldEvent, minAge));
        foldEventType.addMember("maxAge",netCDF::ncDouble,offsetof(FoldEvent, maxAge));
        foldEventType.addMember("periodic",netCDF::ncChar,offsetof(FoldEvent, periodic));
        foldEventType.addMember("wavelength",netCDF::ncDouble,offsetof(FoldEvent, wavelength));
        foldEventType.addMember("amplitude",netCDF::ncDouble,offsetof(FoldEvent, amplitude));
        foldEventType.addMember("asymmetry",netCDF::ncChar,offsetof(FoldEvent, asymmetry));
        foldEventType.addMember("asymmetryShift",netCDF::ncDouble,offsetof(FoldEvent, asymmetryShift));
        foldEventType.addMember("secondaryWavelength",netCDF::ncDouble,offsetof(FoldEvent, secondaryWavelength));
        foldEventType.addMember("secondaryAmplitude",netCDF::ncDouble,offsetof(FoldEvent, secondaryAmplitude));
        foldEventType.addMember("enabled",netCDF::ncChar,offsetof(FoldEvent, enabled));
        foldEventType.addMember("name",netCDF::ncChar,offsetof(FoldEvent, name),length);
        eventLogGroup.addVar("FoldEvents",foldEventType,foldEventIndex);

        // Create fold observation compound type and variable
        netCDF::NcCompoundType foliationEventType = eventLogGroup.addCompoundType("FoliationEvent",sizeof(FoliationEvent));
        foliationEventType.addMember("eventId",netCDF::ncInt,offsetof(FoliationEvent, eventId));
        foliationEventType.addMember("minAge",netCDF::ncDouble,offsetof(FoliationEvent, minAge));
        foliationEventType.addMember("maxAge",netCDF::ncDouble,offsetof(FoliationEvent, maxAge));
        foliationEventType.addMember("lowerScalarValue",netCDF::ncDouble,offsetof(FoliationEvent, lowerScalarValue));
        foliationEventType.addMember("upperScalarValue",netCDF::ncDouble,offsetof(FoliationEvent, upperScalarValue));
        foliationEventType.addMember("enabled",netCDF::ncChar,offsetof(FoliationEvent, enabled));
        foliationEventType.addMember("name",netCDF::ncChar,offsetof(FoliationEvent, name),length);
        eventLogGroup.addVar("FoliationEvents",foliationEventType,foliationEventIndex);

        // Create discontinuity observation compound type and variable
        netCDF::NcCompoundType discontinuityEventType = eventLogGroup.addCompoundType("DiscontinuityEvent",sizeof(DiscontinuityEvent));
        discontinuityEventType.addMember("eventId",netCDF::ncInt,offsetof(DiscontinuityEvent, eventId));
        discontinuityEventType.addMember("minAge",netCDF::ncDouble,offsetof(DiscontinuityEvent, minAge));
        discontinuityEventType.addMember("maxAge",netCDF::ncDouble,offsetof(DiscontinuityEvent, maxAge));
        discontinuityEventType.addMember("scalarValue",netCDF::ncDouble,offsetof(DiscontinuityEvent, scalarValue));
        discontinuityEventType.addMember("enabled",netCDF::ncChar,offsetof(DiscontinuityEvent, enabled));
        discontinuityEventType.addMember("name",netCDF::ncChar,offsetof(DiscontinuityEvent, name),length);
        eventLogGroup.addVar("DiscontinuityEvents",discontinuityEventType,discontinuityEventIndex);
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what() << std::endl;
        resp = createErrorMsg(1,"Failed to create Extracted Information Structure and types",verbose);
    }
    return resp;
}

/*!
 * \brief Retrieves fault event information from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - a reference to where the event data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFaultEvents(netCDF::NcGroup* rootNode, std::vector<FaultEvent>& events, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("ExtractedInformation") != groups.end()) {
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") != eiGroups.end()) {
            netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
            netCDF::NcVar faultEvents = eventLogGroup.getVar("FaultEvents");
            for (size_t i=0;i<eventLogGroup.getDim("FaultEventIndex").getSize();i++) {
                FaultEvent event;
                std::vector<size_t> start; start.push_back(i);
                faultEvents.getVar(start,&event);
                events.push_back(event);
            }
        } else {
            resp = createErrorMsg(1,"No EventLog Group Node Present");
        }
    } else {
        resp = createErrorMsg(1,"No Extracted Information Group Node Present");
    }
    return resp;
}

/*!
 * \brief Retrieves fold event information from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - a reference to where the event data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFoldEvents(netCDF::NcGroup* rootNode, std::vector<FoldEvent>& events, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("ExtractedInformation") != groups.end()) {
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") != eiGroups.end()) {
            netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
            netCDF::NcVar foldEvents = eventLogGroup.getVar("FoldEvents");
            for (size_t i=0;i<eventLogGroup.getDim("FoldEventIndex").getSize();i++) {
                FoldEvent event;
                std::vector<size_t> start; start.push_back(i);
                foldEvents.getVar(start,&event);
                events.push_back(event);
            }
        } else {
            resp = createErrorMsg(1,"No EventLog Group Node Present");
        }
    } else {
        resp = createErrorMsg(1,"No Extracted Information Group Node Present");
    }
    return resp;
}

/*!
 * \brief Retrieves foliation event information from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - a reference to where the event data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFoliationEvents(netCDF::NcGroup* rootNode, std::vector<FoliationEvent>& events, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("ExtractedInformation") != groups.end()) {
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") != eiGroups.end()) {
            netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
            netCDF::NcVar foliationEvents = eventLogGroup.getVar("FoliationEvents");
            for (size_t i=0;i<eventLogGroup.getDim("FoliationEventIndex").getSize();i++) {
                FoliationEvent event;
                std::vector<size_t> start; start.push_back(i);
                foliationEvents.getVar(start,&event);
                events.push_back(event);
            }
        } else {
            resp = createErrorMsg(1,"No EventLog Group Node Present");
        }
    } else {
        resp = createErrorMsg(1,"No Extracted Information Group Node Present");
    }
    return resp;
}

/*!
 * \brief Retrieves discontinuity event information from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - a reference to where the event data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information retrieval with an error message if it failed
 */
LoopProjectFileResponse GetDiscontinuityEvents(netCDF::NcGroup* rootNode, std::vector<DiscontinuityEvent>& events, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("ExtractedInformation") != groups.end()) {
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") != eiGroups.end()) {
            netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
            netCDF::NcVar discontinuityEvents = eventLogGroup.getVar("DiscontinuityEvents");
            for (size_t i=0;i<eventLogGroup.getDim("DiscontinuityEventIndex").getSize();i++) {
                DiscontinuityEvent event;
                std::vector<size_t> start; start.push_back(i);
                discontinuityEvents.getVar(start,&event);
                events.push_back(event);
            }
        } else {
            resp = createErrorMsg(1,"No EventLog Group Node Present");
        }
    } else {
        resp = createErrorMsg(1,"No Extracted Information Group Node Present");
    }
    return resp;
}

/*!
 * \brief Sets fault event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetFaultEvents(netCDF::NcGroup* rootNode, std::vector<FaultEvent> events, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("ExtractedInformation") == groups.end()) {
            rootNode->addGroup("ExtractedInformation");
        }
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") == eiGroups.end()) {
            resp = CreateExtractedInformationGroup(&extractedInformationGroup,verbose);
        }
        netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(events.size());
        netCDF::NcVar faultEvents = eventLogGroup.getVar("FaultEvents");
        faultEvents.putVar(start,count,&(events[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add fault events to loop project file");
    }
    return resp;
}

/*!
 * \brief Sets fold event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoldEvents(netCDF::NcGroup* rootNode, std::vector<FoldEvent> events, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("ExtractedInformation") == groups.end()) {
            rootNode->addGroup("ExtractedInformation");
        }
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") == eiGroups.end()) {
            resp = CreateExtractedInformationGroup(&extractedInformationGroup);
        }
        netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(events.size());
        netCDF::NcVar foldEvents = eventLogGroup.getVar("FoldEvents");
        foldEvents.putVar(start,count,&(events[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add fold events to loop project file");
    }
    return resp;
}

/*!
 * \brief Sets foliation event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoliationEvents(netCDF::NcGroup* rootNode, std::vector<FoliationEvent> events, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("ExtractedInformation") == groups.end()) {
            rootNode->addGroup("ExtractedInformation");
        }
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") == eiGroups.end()) {
            resp = CreateExtractedInformationGroup(&extractedInformationGroup);
        }
        netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(events.size());
        netCDF::NcVar foliationEvents = eventLogGroup.getVar("FoliationEvents");
        foliationEvents.putVar(start,count,&(events[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add foliation events to loop project file");
    }
    return resp;
}

/*!
 * \brief Sets discontinuity event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetDiscontinuityEvents(netCDF::NcGroup* rootNode, std::vector<DiscontinuityEvent> events, bool verbose=false)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("ExtractedInformation") == groups.end()) {
            rootNode->addGroup("ExtractedInformation");
        }
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") == eiGroups.end()) {
            resp = CreateExtractedInformationGroup(&extractedInformationGroup);
        }
        netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(events.size());
        netCDF::NcVar discontinuityEvents = eventLogGroup.getVar("DiscontinuityEvents");
        discontinuityEvents.putVar(start,count,&(events[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add discontinuity events to loop project file");
    }
    return resp;
}

} // namespace ExtractedInformation
} // namespace LoopProjectFile

#endif