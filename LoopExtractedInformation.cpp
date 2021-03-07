#include "LoopExtractedInformation.h"

namespace LoopProjectFile {

bool ExtractedInformation::CheckExtractedInformationValid(netCDF::NcGroup* rootNode, bool verbose)
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

LoopProjectFileResponse ExtractedInformation::CreateExtractedInformationGroup(netCDF::NcGroup* extractedInformationGroup, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    std::vector<int> length; length.push_back(LOOP_NAME_LENGTH);
    std::vector<int> supergroupLength; supergroupLength.push_back(LOOP_SUPERGROUP_NAME_LENGTH);
    std::vector<int> colourLength; colourLength.push_back(7);
    try {
        extractedInformationGroup->addGroup("StratigraphicInformation");
        netCDF::NcGroup stratigraphicInformationGroup = extractedInformationGroup->getGroup("StratigraphicInformation");
        netCDF::NcDim stratigraphicLayerIndex = stratigraphicInformationGroup.addDim("index");

        extractedInformationGroup->addGroup("EventLog");
        netCDF::NcGroup eventLogGroup = extractedInformationGroup->getGroup("EventLog");
        netCDF::NcDim faultEventIndex = eventLogGroup.addDim("faultEventIndex");
        netCDF::NcDim foldEventIndex = eventLogGroup.addDim("foldEventIndex");
        netCDF::NcDim foliationEventIndex = eventLogGroup.addDim("foliationEventIndex");
        netCDF::NcDim discontinuityEventIndex = eventLogGroup.addDim("discontinuityEventIndex");

        // Create stratigraphic layer compound type and variable
        netCDF::NcCompoundType stratigraphicLayerType = stratigraphicInformationGroup.addCompoundType("StratigraphicLayer",sizeof(StratigraphicLayer));
        stratigraphicLayerType.addMember("eventId",netCDF::ncInt,offsetof(StratigraphicLayer, eventId));
        stratigraphicLayerType.addMember("minAge",netCDF::ncDouble,offsetof(StratigraphicLayer, minAge));
        stratigraphicLayerType.addMember("maxAge",netCDF::ncDouble,offsetof(StratigraphicLayer, maxAge));
        stratigraphicLayerType.addMember("name",netCDF::ncChar,offsetof(StratigraphicLayer, name),length);
        stratigraphicLayerType.addMember("supergroup",netCDF::ncChar,offsetof(StratigraphicLayer, supergroup),supergroupLength);
        stratigraphicLayerType.addMember("enabled",netCDF::ncChar,offsetof(StratigraphicLayer, enabled));
        stratigraphicLayerType.addMember("rank",netCDF::ncInt,offsetof(StratigraphicLayer, rank));
        stratigraphicLayerType.addMember("type",netCDF::ncInt,offsetof(StratigraphicLayer, type));
        stratigraphicLayerType.addMember("thickness",netCDF::ncDouble,offsetof(StratigraphicLayer, thickness));
        stratigraphicLayerType.addMember("colour1Red",netCDF::ncUbyte,offsetof(StratigraphicLayer, colour1Red));
        stratigraphicLayerType.addMember("colour1Green",netCDF::ncUbyte,offsetof(StratigraphicLayer, colour1Green));
        stratigraphicLayerType.addMember("colour1Blue",netCDF::ncUbyte,offsetof(StratigraphicLayer, colour1Blue));
        stratigraphicLayerType.addMember("colour2Red",netCDF::ncUbyte,offsetof(StratigraphicLayer, colour2Red));
        stratigraphicLayerType.addMember("colour2Green",netCDF::ncUbyte,offsetof(StratigraphicLayer, colour2Green));
        stratigraphicLayerType.addMember("colour2Blue",netCDF::ncUbyte,offsetof(StratigraphicLayer, colour2Blue));
        stratigraphicInformationGroup.addVar("stratigraphicLayers",stratigraphicLayerType,stratigraphicLayerIndex);

        // Create fault event compound type and variable
        netCDF::NcCompoundType faultEventType = eventLogGroup.addCompoundType("FaultEvent",sizeof(FaultEvent));
        faultEventType.addMember("eventId",netCDF::ncInt,offsetof(FaultEvent, eventId));
        faultEventType.addMember("minAge",netCDF::ncDouble,offsetof(FaultEvent, minAge));
        faultEventType.addMember("maxAge",netCDF::ncDouble,offsetof(FaultEvent, maxAge));
        faultEventType.addMember("name",netCDF::ncChar,offsetof(FaultEvent, name),length);
        faultEventType.addMember("supergroup",netCDF::ncChar,offsetof(FaultEvent, supergroup),supergroupLength);
        faultEventType.addMember("enabled",netCDF::ncChar,offsetof(FaultEvent, enabled));
        faultEventType.addMember("rank",netCDF::ncInt,offsetof(FaultEvent, rank));
        faultEventType.addMember("type",netCDF::ncInt,offsetof(FaultEvent, type));
        faultEventType.addMember("avgDisplacement",netCDF::ncDouble,offsetof(FaultEvent, avgDisplacement));
        faultEventType.addMember("avgDownthrowDir",netCDF::ncDouble,offsetof(FaultEvent, avgDownthrowDir));
        faultEventType.addMember("influenceDistance",netCDF::ncDouble,offsetof(FaultEvent, influenceDistance));
        faultEventType.addMember("verticalRadius",netCDF::ncDouble,offsetof(FaultEvent, verticalRadius));
        faultEventType.addMember("horizontalRadius",netCDF::ncDouble,offsetof(FaultEvent, horizontalRadius));
        faultEventType.addMember("colour",netCDF::ncChar,offsetof(FaultEvent, colour),colourLength);
        eventLogGroup.addVar("faultEvents",faultEventType,faultEventIndex);

        // Create fold event compound type and variable
        netCDF::NcCompoundType foldEventType = eventLogGroup.addCompoundType("FoldEvent",sizeof(FoldEvent));
        foldEventType.addMember("eventId",netCDF::ncInt,offsetof(FoldEvent, eventId));
        foldEventType.addMember("minAge",netCDF::ncDouble,offsetof(FoldEvent, minAge));
        foldEventType.addMember("maxAge",netCDF::ncDouble,offsetof(FoldEvent, maxAge));
        foldEventType.addMember("name",netCDF::ncChar,offsetof(FoldEvent, name),length);
        foldEventType.addMember("supergroup",netCDF::ncChar,offsetof(FoldEvent, supergroup),supergroupLength);
        foldEventType.addMember("enabled",netCDF::ncChar,offsetof(FoldEvent, enabled));
        foldEventType.addMember("rank",netCDF::ncInt,offsetof(FoldEvent, rank));
        foldEventType.addMember("type",netCDF::ncInt,offsetof(FoldEvent, type));
        foldEventType.addMember("periodic",netCDF::ncChar,offsetof(FoldEvent, periodic));
        foldEventType.addMember("wavelength",netCDF::ncDouble,offsetof(FoldEvent, wavelength));
        foldEventType.addMember("amplitude",netCDF::ncDouble,offsetof(FoldEvent, amplitude));
        foldEventType.addMember("asymmetry",netCDF::ncChar,offsetof(FoldEvent, asymmetry));
        foldEventType.addMember("asymmetryShift",netCDF::ncDouble,offsetof(FoldEvent, asymmetryShift));
        foldEventType.addMember("secondaryWavelength",netCDF::ncDouble,offsetof(FoldEvent, secondaryWavelength));
        foldEventType.addMember("secondaryAmplitude",netCDF::ncDouble,offsetof(FoldEvent, secondaryAmplitude));
        eventLogGroup.addVar("foldEvents",foldEventType,foldEventIndex);

        // Create foliation event compound type and variable
        netCDF::NcCompoundType foliationEventType = eventLogGroup.addCompoundType("FoliationEvent",sizeof(FoliationEvent));
        foliationEventType.addMember("eventId",netCDF::ncInt,offsetof(FoliationEvent, eventId));
        foliationEventType.addMember("minAge",netCDF::ncDouble,offsetof(FoliationEvent, minAge));
        foliationEventType.addMember("maxAge",netCDF::ncDouble,offsetof(FoliationEvent, maxAge));
        foliationEventType.addMember("name",netCDF::ncChar,offsetof(FoliationEvent, name),length);
        foliationEventType.addMember("supergroup",netCDF::ncChar,offsetof(FoliationEvent, supergroup),supergroupLength);
        foliationEventType.addMember("enabled",netCDF::ncChar,offsetof(FoliationEvent, enabled));
        foliationEventType.addMember("rank",netCDF::ncInt,offsetof(FoliationEvent, rank));
        foliationEventType.addMember("type",netCDF::ncInt,offsetof(FoliationEvent, type));
        foliationEventType.addMember("lowerScalarValue",netCDF::ncDouble,offsetof(FoliationEvent, lowerScalarValue));
        foliationEventType.addMember("upperScalarValue",netCDF::ncDouble,offsetof(FoliationEvent, upperScalarValue));
        eventLogGroup.addVar("foliationEvents",foliationEventType,foliationEventIndex);

        // Create discontinuity event compound type and variable
        netCDF::NcCompoundType discontinuityEventType = eventLogGroup.addCompoundType("DiscontinuityEvent",sizeof(DiscontinuityEvent));
        discontinuityEventType.addMember("eventId",netCDF::ncInt,offsetof(DiscontinuityEvent, eventId));
        discontinuityEventType.addMember("minAge",netCDF::ncDouble,offsetof(DiscontinuityEvent, minAge));
        discontinuityEventType.addMember("maxAge",netCDF::ncDouble,offsetof(DiscontinuityEvent, maxAge));
        discontinuityEventType.addMember("name",netCDF::ncChar,offsetof(DiscontinuityEvent, name),length);
        discontinuityEventType.addMember("supergroup",netCDF::ncChar,offsetof(DiscontinuityEvent, supergroup),supergroupLength);
        discontinuityEventType.addMember("enabled",netCDF::ncChar,offsetof(DiscontinuityEvent, enabled));
        discontinuityEventType.addMember("rank",netCDF::ncInt,offsetof(DiscontinuityEvent, rank));
        discontinuityEventType.addMember("type",netCDF::ncInt,offsetof(DiscontinuityEvent, type));
        discontinuityEventType.addMember("scalarValue",netCDF::ncDouble,offsetof(DiscontinuityEvent, scalarValue));
        eventLogGroup.addVar("discontinuityEvents",discontinuityEventType,discontinuityEventIndex);
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what() << std::endl;
        resp = createErrorMsg(1,"Failed to create Extracted Information Structure and types",verbose);
    }
    return resp;
}

LoopProjectFileResponse ExtractedInformation::GetFaultEvents(netCDF::NcGroup* rootNode, std::vector<FaultEvent>& events, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("ExtractedInformation") != groups.end()) {
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") != eiGroups.end()) {
            netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
            netCDF::NcVar faultEvents = eventLogGroup.getVar("faultEvents");
            for (size_t i=0;i<eventLogGroup.getDim("faultEventIndex").getSize();i++) {
                FaultEvent event;
                std::vector<size_t> start; start.push_back(i);
                faultEvents.getVar(start,&event);
                events.push_back(event);
            }
        } else {
            resp = createErrorMsg(1,"No EventLog Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Extracted Information Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse ExtractedInformation::GetFoldEvents(netCDF::NcGroup* rootNode, std::vector<FoldEvent>& events, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("ExtractedInformation") != groups.end()) {
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") != eiGroups.end()) {
            netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
            netCDF::NcVar foldEvents = eventLogGroup.getVar("foldEvents");
            for (size_t i=0;i<eventLogGroup.getDim("foldEventIndex").getSize();i++) {
                FoldEvent event;
                std::vector<size_t> start; start.push_back(i);
                foldEvents.getVar(start,&event);
                events.push_back(event);
            }
        } else {
            resp = createErrorMsg(1,"No Event Log Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Extracted Information Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse ExtractedInformation::GetFoliationEvents(netCDF::NcGroup* rootNode, std::vector<FoliationEvent>& events, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("ExtractedInformation") != groups.end()) {
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") != eiGroups.end()) {
            netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
            netCDF::NcVar foliationEvents = eventLogGroup.getVar("foliationEvents");
            for (size_t i=0;i<eventLogGroup.getDim("foliationEventIndex").getSize();i++) {
                FoliationEvent event;
                std::vector<size_t> start; start.push_back(i);
                foliationEvents.getVar(start,&event);
                events.push_back(event);
            }
        } else {
            resp = createErrorMsg(1,"No Event Log Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Extracted Information Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse ExtractedInformation::GetDiscontinuityEvents(netCDF::NcGroup* rootNode, std::vector<DiscontinuityEvent>& events, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("ExtractedInformation") != groups.end()) {
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("EventLog") != eiGroups.end()) {
            netCDF::NcGroup eventLogGroup = extractedInformationGroup.getGroup("EventLog");
            netCDF::NcVar discontinuityEvents = eventLogGroup.getVar("discontinuityEvents");
            for (size_t i=0;i<eventLogGroup.getDim("discontinuityEventIndex").getSize();i++) {
                DiscontinuityEvent event;
                std::vector<size_t> start; start.push_back(i);
                discontinuityEvents.getVar(start,&event);
                events.push_back(event);
            }
        } else {
            resp = createErrorMsg(1,"No Event Log Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Extracted Information Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse ExtractedInformation::GetStratigraphicLayers(netCDF::NcGroup* rootNode, std::vector<StratigraphicLayer>& layers, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("ExtractedInformation") != groups.end()) {
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("StratigraphicInformation") != eiGroups.end()) {
            netCDF::NcGroup stratigraphicInformationGroup = extractedInformationGroup.getGroup("StratigraphicInformation");
            netCDF::NcVar stratigraphicLayers = stratigraphicInformationGroup.getVar("stratigraphicLayers");
            for (size_t i=0;i<stratigraphicInformationGroup.getDim("index").getSize();i++) {
                StratigraphicLayer layer;
                std::vector<size_t> start; start.push_back(i);
                stratigraphicLayers.getVar(start,&layer);
                layers.push_back(layer);
            }
        } else {
            resp = createErrorMsg(1,"No Stratigraphic Information Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Extracted Information Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse ExtractedInformation::SetFaultEvents(netCDF::NcGroup* rootNode, std::vector<FaultEvent> events, bool verbose)
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
        netCDF::NcVar faultEvents = eventLogGroup.getVar("faultEvents");
        faultEvents.putVar(start,count,&(events[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add fault events to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse ExtractedInformation::SetFoldEvents(netCDF::NcGroup* rootNode, std::vector<FoldEvent> events, bool verbose)
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
        netCDF::NcVar foldEvents = eventLogGroup.getVar("foldEvents");
        foldEvents.putVar(start,count,&(events[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add fold events to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse ExtractedInformation::SetFoliationEvents(netCDF::NcGroup* rootNode, std::vector<FoliationEvent> events, bool verbose)
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
        netCDF::NcVar foliationEvents = eventLogGroup.getVar("foliationEvents");
        foliationEvents.putVar(start,count,&(events[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add foliation events to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse ExtractedInformation::SetDiscontinuityEvents(netCDF::NcGroup* rootNode, std::vector<DiscontinuityEvent> events, bool verbose)
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
        netCDF::NcVar discontinuityEvents = eventLogGroup.getVar("discontinuityEvents");
        discontinuityEvents.putVar(start,count,&(events[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add discontinuity events to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse ExtractedInformation::SetStratigraphicLayers(netCDF::NcGroup* rootNode, std::vector<StratigraphicLayer> layers, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("ExtractedInformation") == groups.end()) {
            rootNode->addGroup("ExtractedInformation");
        }
        netCDF::NcGroup extractedInformationGroup = rootNode->getGroup("ExtractedInformation");
        auto eiGroups = extractedInformationGroup.getGroups();
        if (eiGroups.find("StratigraphicInformation") == eiGroups.end()) {
            resp = CreateExtractedInformationGroup(&extractedInformationGroup);
        }
        netCDF::NcGroup stratigraphicInformationGroup = extractedInformationGroup.getGroup("StratigraphicInformation");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(layers.size());
        netCDF::NcVar stratigraphicLayers = stratigraphicInformationGroup.getVar("stratigraphicLayers");
        stratigraphicLayers.putVar(start,count,&(layers[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add stratigraphic layers to loop project file",verbose);
    }
    return resp;
}

} // namespace LoopProjectFile