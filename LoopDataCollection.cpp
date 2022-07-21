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
    std::vector<int> loopNameLength; loopNameLength.push_back(LOOP_NAME_LENGTH);
    try {
        dataCollectionGroup->addGroup("Observations");
        netCDF::NcGroup observationGroup = dataCollectionGroup->getGroup("Observations");
        netCDF::NcDim faultIndex = observationGroup.addDim("faultObservationIndex");
        netCDF::NcDim foldIndex = observationGroup.addDim("foldObservationIndex");
        netCDF::NcDim foliationIndex = observationGroup.addDim("foliationObservationIndex");
        netCDF::NcDim discontinuityIndex = observationGroup.addDim("discontinuityObservationIndex");
        netCDF::NcDim stratigraphicIndex = observationGroup.addDim("stratigraphicObservationIndex");

        // Create fault observation compound type and variable
        netCDF::NcCompoundType faultObservationType = observationGroup.addCompoundType("faultObservation",sizeof(FaultObservation));
        faultObservationType.addMember("eventId",netCDF::ncInt,offsetof(FaultObservation, eventId));
        faultObservationType.addMember("easting",netCDF::ncDouble,offsetof(FaultObservation, easting));
        faultObservationType.addMember("northing",netCDF::ncDouble,offsetof(FaultObservation, northing));
        faultObservationType.addMember("altitude",netCDF::ncDouble,offsetof(FaultObservation, altitude));
        faultObservationType.addMember("type",netCDF::ncInt,offsetof(FaultObservation, type));
        faultObservationType.addMember("dipdir",netCDF::ncDouble,offsetof(FaultObservation, dipdir));
        faultObservationType.addMember("dip",netCDF::ncDouble,offsetof(FaultObservation, dip));
        faultObservationType.addMember("dipPolarity",netCDF::ncDouble,offsetof(FaultObservation, dipPolarity));
        faultObservationType.addMember("val",netCDF::ncDouble,offsetof(FaultObservation, val));
        faultObservationType.addMember("displacement",netCDF::ncDouble,offsetof(FaultObservation, displacement));
        faultObservationType.addMember("posOnly",netCDF::ncChar,offsetof(FaultObservation, posOnly));
        observationGroup.addVar("faultObservations",faultObservationType,faultIndex);

        // Create fold observation compound type and variable
        netCDF::NcCompoundType foldObservationType = observationGroup.addCompoundType("foldObservation",sizeof(FoldObservation));
        foldObservationType.addMember("eventId",netCDF::ncInt,offsetof(FoldObservation, eventId));
        foldObservationType.addMember("easting",netCDF::ncDouble,offsetof(FoldObservation, easting));
        foldObservationType.addMember("northing",netCDF::ncDouble,offsetof(FoldObservation, northing));
        foldObservationType.addMember("altitude",netCDF::ncDouble,offsetof(FoldObservation, altitude));
        foldObservationType.addMember("type",netCDF::ncInt,offsetof(FoldObservation, type));
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
        foliationObservationType.addMember("type",netCDF::ncInt,offsetof(FoliationObservation, type));
        foliationObservationType.addMember("dipdir",netCDF::ncDouble,offsetof(FoliationObservation, dipdir));
        foliationObservationType.addMember("dip",netCDF::ncDouble,offsetof(FoliationObservation, dip));
        observationGroup.addVar("foliationObservations",foliationObservationType,foliationIndex);

        // Create discontinuity observation compound type and variable
        netCDF::NcCompoundType discontinuityObservationType = observationGroup.addCompoundType("discontinuityObservation",sizeof(DiscontinuityObservation));
        discontinuityObservationType.addMember("eventId",netCDF::ncInt,offsetof(DiscontinuityObservation, eventId));
        discontinuityObservationType.addMember("easting",netCDF::ncDouble,offsetof(DiscontinuityObservation, easting));
        discontinuityObservationType.addMember("northing",netCDF::ncDouble,offsetof(DiscontinuityObservation, northing));
        discontinuityObservationType.addMember("altitude",netCDF::ncDouble,offsetof(DiscontinuityObservation, altitude));
        discontinuityObservationType.addMember("type",netCDF::ncInt,offsetof(DiscontinuityObservation, type));
        discontinuityObservationType.addMember("dipdir",netCDF::ncDouble,offsetof(DiscontinuityObservation, dipdir));
        discontinuityObservationType.addMember("dip",netCDF::ncDouble,offsetof(DiscontinuityObservation, dip));
        observationGroup.addVar("discontinuityObservations",discontinuityObservationType,discontinuityIndex);

        // Create stratigraphic observation compound type and variable
        netCDF::NcCompoundType stratigraphicObservationType = observationGroup.addCompoundType("stratigraphicObservation",sizeof(StratigraphicObservation));
        stratigraphicObservationType.addMember("eventId",netCDF::ncInt,offsetof(StratigraphicObservation, eventId));
        stratigraphicObservationType.addMember("easting",netCDF::ncDouble,offsetof(StratigraphicObservation, easting));
        stratigraphicObservationType.addMember("northing",netCDF::ncDouble,offsetof(StratigraphicObservation, northing));
        stratigraphicObservationType.addMember("altitude",netCDF::ncDouble,offsetof(StratigraphicObservation, altitude));
        stratigraphicObservationType.addMember("type",netCDF::ncInt,offsetof(StratigraphicObservation, type));
        stratigraphicObservationType.addMember("dipdir",netCDF::ncDouble,offsetof(StratigraphicObservation, dipdir));
        stratigraphicObservationType.addMember("dip",netCDF::ncDouble,offsetof(StratigraphicObservation, dip));
        stratigraphicObservationType.addMember("dipPolarity",netCDF::ncDouble,offsetof(StratigraphicObservation, dipPolarity));
        stratigraphicObservationType.addMember("layer",netCDF::ncChar,offsetof(StratigraphicObservation, layer),loopNameLength);
        observationGroup.addVar("stratigraphicObservations",stratigraphicObservationType,stratigraphicIndex);
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what() << std::endl;
        resp = createErrorMsg(1,"Failed to create Data Collection Group and Types",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::CreateContactGroup(netCDF::NcGroup* dataCollectionGroup, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        dataCollectionGroup->addGroup("Contacts");
        netCDF::NcGroup contactGroup = dataCollectionGroup->getGroup("Contacts");
        netCDF::NcDim contactIndex = contactGroup.addDim("index");

        // Create fault observation compound type and variable
        netCDF::NcCompoundType contactObservationType = contactGroup.addCompoundType("contactObservation",sizeof(ContactObservation));
        contactObservationType.addMember("eventId",netCDF::ncInt,offsetof(ContactObservation, eventId));
        contactObservationType.addMember("easting",netCDF::ncDouble,offsetof(ContactObservation, easting));
        contactObservationType.addMember("northing",netCDF::ncDouble,offsetof(ContactObservation, northing));
        contactObservationType.addMember("altitude",netCDF::ncDouble,offsetof(ContactObservation, altitude));
        contactObservationType.addMember("type",netCDF::ncInt,offsetof(ContactObservation, type));
        contactGroup.addVar("contacts",contactObservationType,contactIndex);
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what() << std::endl;
        resp = createErrorMsg(1,"Failed to create Data Collection Group Contacts",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::CreateDrillholeGroup(netCDF::NcGroup* dataCollectionGroup, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    std::vector<int> drillholePropertyCodeLength; drillholePropertyCodeLength.push_back(LOOP_DRILLHOLE_PROPERTY_CODE_LENGTH);
    std::vector<int> drillholeProperty1Length; drillholeProperty1Length.push_back(LOOP_DRILLHOLE_PROPERTY1_LENGTH);
    std::vector<int> drillholeProperty2Length; drillholeProperty2Length.push_back(LOOP_DRILLHOLE_PROPERTY2_LENGTH);
    std::vector<int> drillholeUnitLength; drillholeUnitLength.push_back(LOOP_DRILLHOLE_UNIT_LENGTH);
    std::vector<int> drillholePropertyNameLength; drillholeUnitLength.push_back(LOOP_DRILLHOLE_PROPERTY_NAME_LENGTH);
    std::vector<int> drillholePropertyValueLength; drillholeUnitLength.push_back(LOOP_DRILLHOLE_PROPERTY_VALUE_LENGTH);
    std::vector<int> drillholeSurveyUnitLength; drillholeSurveyUnitLength.push_back(LOOP_DRILLHOLE_SURVEY_UNIT_LENGTH);
    try {
        dataCollectionGroup->addGroup("Drillholes");
        netCDF::NcGroup drillholeGroup = dataCollectionGroup->getGroup("Drillholes");
        netCDF::NcDim drillholeObservationIndex = drillholeGroup.addDim("drillholeObservationIndex");
        netCDF::NcDim drillholePropertyIndex = drillholeGroup.addDim("drillholePropertyIndex");
        netCDF::NcDim drillholeSurveyIndex = drillholeGroup.addDim("drillholeSurveyIndex");

        // Create drillhole observation compound type and variable
        netCDF::NcCompoundType drillholeObservationType = drillholeGroup.addCompoundType("drillholeObservation",sizeof(DrillholeObservation));
        drillholeObservationType.addMember("collarId",netCDF::ncInt,offsetof(DrillholeObservation, eventId));
        drillholeObservationType.addMember("fromEasting",netCDF::ncDouble,offsetof(DrillholeObservation, easting));
        drillholeObservationType.addMember("fromNorthing",netCDF::ncDouble,offsetof(DrillholeObservation, northing));
        drillholeObservationType.addMember("fromAltitude",netCDF::ncDouble,offsetof(DrillholeObservation, altitude));
        drillholeObservationType.addMember("type",netCDF::ncInt,offsetof(DrillholeObservation, type));
        drillholeObservationType.addMember("toEasting",netCDF::ncDouble,offsetof(DrillholeObservation, toEasting));
        drillholeObservationType.addMember("toNorthing",netCDF::ncDouble,offsetof(DrillholeObservation, toNorthing));
        drillholeObservationType.addMember("toAltitude",netCDF::ncDouble,offsetof(DrillholeObservation, toAltitude));
        drillholeObservationType.addMember("from",netCDF::ncDouble,offsetof(DrillholeObservation, from));
        drillholeObservationType.addMember("to",netCDF::ncDouble,offsetof(DrillholeObservation, to));
        drillholeObservationType.addMember("propertyCode",netCDF::ncChar,offsetof(DrillholeObservation, propertyCode),drillholePropertyCodeLength);
        drillholeObservationType.addMember("property1",netCDF::ncChar,offsetof(DrillholeObservation, property1),drillholeProperty1Length);
        drillholeObservationType.addMember("property2",netCDF::ncChar,offsetof(DrillholeObservation, property2),drillholeProperty2Length);
        drillholeObservationType.addMember("unit",netCDF::ncChar,offsetof(DrillholeObservation, unit),drillholeUnitLength);
        drillholeGroup.addVar("drillholeObservations",drillholeObservationType,drillholeObservationIndex);

        // Create drillhole property compound type and variable
        netCDF::NcCompoundType drillholePropertyType = drillholeGroup.addCompoundType("drillholeProperty",sizeof(DrillholeProperty));
        drillholePropertyType.addMember("collarId",netCDF::ncInt,offsetof(DrillholeProperty, collarId));
        drillholePropertyType.addMember("propertyName",netCDF::ncChar,offsetof(DrillholeProperty, propertyName));
        drillholePropertyType.addMember("propertyValue",netCDF::ncChar,offsetof(DrillholeProperty, propertyValue));
        drillholeGroup.addVar("drillholeProperties",drillholePropertyType,drillholePropertyIndex);

        // Create drillhole property compound type and variable
        netCDF::NcCompoundType drillholeSurveyType = drillholeGroup.addCompoundType("drillholeSurvey",sizeof(DrillholeSurvey));
        drillholeSurveyType.addMember("collarId",netCDF::ncInt,offsetof(DrillholeSurvey, collarId));
        drillholeSurveyType.addMember("depth",netCDF::ncDouble,offsetof(DrillholeSurvey, depth));
        drillholeSurveyType.addMember("angle1",netCDF::ncDouble,offsetof(DrillholeSurvey, angle1));
        drillholeSurveyType.addMember("angle2",netCDF::ncDouble,offsetof(DrillholeSurvey, angle2));
        drillholeSurveyType.addMember("unit",netCDF::ncChar,offsetof(DrillholeSurvey, unit), drillholeSurveyUnitLength);
        drillholeGroup.addVar("drillholeSurveys",drillholeSurveyType,drillholeSurveyIndex);

    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what() << std::endl;
        resp = createErrorMsg(1,"Failed to create Data Collection Group Drillholes",verbose);
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
            for (size_t i=0;i<observationGroup.getDim("faultObservationIndex").getSize();i++) {
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
            for (size_t i=0;i<observationGroup.getDim("foldObservationIndex").getSize();i++) {
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
            for (size_t i=0;i<observationGroup.getDim("foliationObservationIndex").getSize();i++) {
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
            for (size_t i=0;i<observationGroup.getDim("discontinuityObservationIndex").getSize();i++) {
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

LoopProjectFileResponse DataCollection::GetStratigraphicObservations(netCDF::NcGroup* rootNode, std::vector<StratigraphicObservation>& observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Observations") != dcGroups.end()) {
            netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Observations");
            netCDF::NcVar stratigraphicObs = observationGroup.getVar("stratigraphicObservations");
            for (size_t i=0;i<observationGroup.getDim("stratigraphicObservationIndex").getSize();i++) {
                StratigraphicObservation obs;
                std::vector<size_t> start; start.push_back(i);
                stratigraphicObs.getVar(start,&obs);
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

LoopProjectFileResponse DataCollection::GetContactObservations(netCDF::NcGroup* rootNode, std::vector<ContactObservation>& observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Contacts") != dcGroups.end()) {
            netCDF::NcGroup contactGroup = dataCollectionGroup.getGroup("Contacts");
            netCDF::NcVar contactObs = contactGroup.getVar("contacts");
            for (size_t i=0;i<contactGroup.getDim("index").getSize();i++) {
                ContactObservation obs;
                std::vector<size_t> start; start.push_back(i);
                contactObs.getVar(start,&obs);
                observations.push_back(obs);
            }
        } else {
            resp = createErrorMsg(1,"No Contacts Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::GetDrillholeObservations(netCDF::NcGroup* rootNode, std::vector<DrillholeObservation>& observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Drillholes") != dcGroups.end()) {
            netCDF::NcGroup drillholeGroup = dataCollectionGroup.getGroup("Drillholes");
            netCDF::NcVar drillholeObs = drillholeGroup.getVar("drillholeObservations");
            for (size_t i=0;i<drillholeGroup.getDim("drillholeObservationIndex").getSize();i++) {
                DrillholeObservation obs;
                std::vector<size_t> start; start.push_back(i);
                drillholeObs.getVar(start,&obs);
                observations.push_back(obs);
            }
        } else {
            resp = createErrorMsg(1,"No Drillholes Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present",verbose);
    }
    return resp;
}
LoopProjectFileResponse DataCollection::GetDrillholeProperties(netCDF::NcGroup* rootNode, std::vector<DrillholeProperty>& properties, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Drillholes") != dcGroups.end()) {
            netCDF::NcGroup drillholeGroup = dataCollectionGroup.getGroup("Drillholes");
            netCDF::NcVar drillholeProp = drillholeGroup.getVar("drillholeProperties");
            for (size_t i=0;i<drillholeGroup.getDim("drillholePropertyIndex").getSize();i++) {
                DrillholeProperty prop;
                std::vector<size_t> start; start.push_back(i);
                drillholeProp.getVar(start,&prop);
                properties.push_back(prop);
            }
        } else {
            resp = createErrorMsg(1,"No Drillholes Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present",verbose);
    }
    return resp;
}
LoopProjectFileResponse DataCollection::GetDrillholeSurveys(netCDF::NcGroup* rootNode, std::vector<DrillholeSurvey>& surveys, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Drillholes") != dcGroups.end()) {
            netCDF::NcGroup drillholeGroup = dataCollectionGroup.getGroup("Drillholes");
            netCDF::NcVar drillholeSurveys = drillholeGroup.getVar("drillholeSurveys");
            for (size_t i=0;i<drillholeGroup.getDim("drillholeSurveyIndex").getSize();i++) {
                DrillholeSurvey survey;
                std::vector<size_t> start; start.push_back(i);
                drillholeSurveys.getVar(start,&survey);
                surveys.push_back(survey);
            }
        } else {
            resp = createErrorMsg(1,"No Drillholes Group Node Present",verbose);
        }
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::GetDataCollectionConfiguration(netCDF::NcGroup* rootNode, DataCollectionConfiguration& configuration, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        dataCollectionGroup.getAtt("quietMode").getValues(&configuration.quietMode);
        dataCollectionGroup.getAtt("deposits").getValues(&configuration.deposits);
        dataCollectionGroup.getAtt("dtb").getValues(&configuration.dtb);
        dataCollectionGroup.getAtt("orientationDecimate").getValues(&configuration.orientationDecimate);
        dataCollectionGroup.getAtt("contactDecimate").getValues(&configuration.contactDecimate);
        dataCollectionGroup.getAtt("intrusionMode").getValues(&configuration.intrusionMode);
        dataCollectionGroup.getAtt("interpolationSpacing").getValues(&configuration.interpolationSpacing);
        dataCollectionGroup.getAtt("misorientation").getValues(&configuration.misorientation);
        dataCollectionGroup.getAtt("interpolationScheme").getValues(&configuration.interpolationScheme);
        dataCollectionGroup.getAtt("faultDecimate").getValues(&configuration.faultDecimate);
        dataCollectionGroup.getAtt("minFaultLength").getValues(&configuration.minFaultLength);
        dataCollectionGroup.getAtt("faultDip").getValues(&configuration.faultDip);
        dataCollectionGroup.getAtt("plutonDip").getValues(&configuration.plutonDip);
        dataCollectionGroup.getAtt("plutonForm").getValues(&configuration.plutonForm);
        dataCollectionGroup.getAtt("distBuffer").getValues(&configuration.distBuffer);
        dataCollectionGroup.getAtt("contactDip").getValues(&configuration.contactDip);
        dataCollectionGroup.getAtt("contactOrientationDecimate").getValues(&configuration.contactOrientationDecimate);
        dataCollectionGroup.getAtt("nullScheme").getValues(&configuration.nullScheme);
        dataCollectionGroup.getAtt("thicknessBuffer").getValues(&configuration.thicknessBuffer);
        dataCollectionGroup.getAtt("maxThicknessAllowed").getValues(&configuration.maxThicknessAllowed);
        dataCollectionGroup.getAtt("foldDecimate").getValues(&configuration.foldDecimate);
        dataCollectionGroup.getAtt("fatStep").getValues(&configuration.fatStep);
        dataCollectionGroup.getAtt("closeDip").getValues(&configuration.closeDip);
        dataCollectionGroup.getAtt("useInterpolations").getValues(&configuration.useInterpolations);
        dataCollectionGroup.getAtt("useFat").getValues(&configuration.useFat);
    } else {
        resp = createErrorMsg(1,"No Data Collection Group Node Present",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::GetDataCollectionSources(netCDF::NcGroup* rootNode, DataCollectionSources& sources, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    auto groups = rootNode->getGroups();
    if (groups.find("DataCollection") != groups.end()) {
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        dataCollectionGroup.getAtt("structureUrl").getValues(&sources.structureUrl);
        dataCollectionGroup.getAtt("geologyUrl").getValues(&sources.geologyUrl);
        dataCollectionGroup.getAtt("faultUrl").getValues(&sources.faultUrl);
        dataCollectionGroup.getAtt("foldUrl").getValues(&sources.foldUrl);
        dataCollectionGroup.getAtt("mindepUrl").getValues(&sources.mindepUrl);
        dataCollectionGroup.getAtt("metadataUrl").getValues(&sources.metadataUrl);
        dataCollectionGroup.getAtt("sourceTags").getValues(&sources.sourceTags);
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

LoopProjectFileResponse DataCollection::SetStratigraphicObservations(netCDF::NcGroup* rootNode, std::vector<StratigraphicObservation> observations, bool verbose)
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
        netCDF::NcVar stratigraphicObs = observationGroup.getVar("stratigraphicObservations");
        stratigraphicObs.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add stratigraphic data to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::SetContactObservations(netCDF::NcGroup* rootNode, std::vector<ContactObservation> observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("DataCollection") == groups.end()) {
            rootNode->addGroup("DataCollection");
        }
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Contacts") == dcGroups.end()) {
            resp = CreateContactGroup(&dataCollectionGroup);
        }
        netCDF::NcGroup contactsGroup = dataCollectionGroup.getGroup("Contacts");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(observations.size());
        netCDF::NcVar contacts = contactsGroup.getVar("contacts");
        contacts.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add stratigraphic contacts data to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::SetDrillholeObservations(netCDF::NcGroup* rootNode, std::vector<DrillholeObservation> observations, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("DataCollection") == groups.end()) {
            rootNode->addGroup("DataCollection");
        }
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Drillholes") == dcGroups.end()) {
            resp = CreateDrillholeGroup(&dataCollectionGroup);
        }
        netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Drillholes");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(observations.size());
        netCDF::NcVar drillholeObs = observationGroup.getVar("drillholeObservations");
        drillholeObs.putVar(start,count,&(observations[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add drillhole data to loop project file",verbose);
    }
    return resp;
}
LoopProjectFileResponse DataCollection::SetDrillholeProperties(netCDF::NcGroup* rootNode, std::vector<DrillholeProperty> properties, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("DataCollection") == groups.end()) {
            rootNode->addGroup("DataCollection");
        }
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Drillholes") == dcGroups.end()) {
            resp = CreateDrillholeGroup(&dataCollectionGroup);
        }
        netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Drillholes");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(properties.size());
        netCDF::NcVar drillholeProperties = observationGroup.getVar("drillholeProperties");
        drillholeProperties.putVar(start,count,&(properties[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add drillhole data to loop project file",verbose);
    }
    return resp;
}
LoopProjectFileResponse DataCollection::SetDrillholeSurveys(netCDF::NcGroup* rootNode, std::vector<DrillholeSurvey> surveys, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("DataCollection") == groups.end()) {
            rootNode->addGroup("DataCollection");
        }
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        auto dcGroups = dataCollectionGroup.getGroups();
        if (dcGroups.find("Drillholes") == dcGroups.end()) {
            resp = CreateDrillholeGroup(&dataCollectionGroup);
        }
        netCDF::NcGroup observationGroup = dataCollectionGroup.getGroup("Drillholes");
        std::vector<size_t> start; start.push_back(0);
        std::vector<size_t> count; count.push_back(surveys.size());
        netCDF::NcVar drillholeSurveys = observationGroup.getVar("drillholeSurveys");
        drillholeSurveys.putVar(start,count,&(surveys[0]));
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add drillhole data to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::SetDataCollectionConfiguration(netCDF::NcGroup* rootNode, DataCollectionConfiguration configuration, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("DataCollection") == groups.end()) {
            rootNode->addGroup("DataCollection");
        }
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        dataCollectionGroup.putAtt("quietMode",netCDF::ncInt,configuration.quietMode);
        dataCollectionGroup.putAtt("deposits",netCDF::ncChar,LOOP_CONFIGURATION_DEFAULT_STRING_LENGTH,configuration.deposits);
        dataCollectionGroup.putAtt("dtb",netCDF::ncChar,LOOP_CONFIGURATION_DEFAULT_STRING_LENGTH,configuration.dtb);
        dataCollectionGroup.putAtt("orientationDecimate",netCDF::ncInt,configuration.orientationDecimate);
        dataCollectionGroup.putAtt("contactDecimate",netCDF::ncInt,configuration.contactDecimate);
        dataCollectionGroup.putAtt("intrusionMode",netCDF::ncInt,configuration.intrusionMode);
        dataCollectionGroup.putAtt("interpolationSpacing",netCDF::ncInt,configuration.interpolationSpacing);
        dataCollectionGroup.putAtt("misorientation",netCDF::ncInt,configuration.misorientation);
        dataCollectionGroup.putAtt("interpolationScheme",netCDF::ncChar,120,configuration.interpolationScheme);
        dataCollectionGroup.putAtt("faultDecimate",netCDF::ncInt,configuration.faultDecimate);
        dataCollectionGroup.putAtt("minFaultLength",netCDF::ncDouble,configuration.minFaultLength);
        dataCollectionGroup.putAtt("faultDip",netCDF::ncDouble,configuration.faultDip);
        dataCollectionGroup.putAtt("plutonDip",netCDF::ncDouble,configuration.plutonDip);
        dataCollectionGroup.putAtt("plutonForm",netCDF::ncChar,120,configuration.plutonForm);
        dataCollectionGroup.putAtt("distBuffer",netCDF::ncDouble,configuration.distBuffer);
        dataCollectionGroup.putAtt("contactDip",netCDF::ncDouble,configuration.contactDip);
        dataCollectionGroup.putAtt("contactOrientationDecimate",netCDF::ncInt,configuration.contactOrientationDecimate);
        dataCollectionGroup.putAtt("nullScheme",netCDF::ncChar,120,configuration.nullScheme);
        dataCollectionGroup.putAtt("thicknessBuffer",netCDF::ncDouble,configuration.thicknessBuffer);
        dataCollectionGroup.putAtt("maxThicknessAllowed",netCDF::ncDouble,configuration.maxThicknessAllowed);
        dataCollectionGroup.putAtt("foldDecimate",netCDF::ncInt,configuration.foldDecimate);
        dataCollectionGroup.putAtt("fatStep",netCDF::ncDouble,configuration.fatStep);
        dataCollectionGroup.putAtt("closeDip",netCDF::ncDouble,configuration.closeDip);
        dataCollectionGroup.putAtt("useInterpolations",netCDF::ncInt,configuration.useInterpolations);
        dataCollectionGroup.putAtt("useFat",netCDF::ncInt,configuration.useFat);
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add data collection configuration data to loop project file",verbose);
    }
    return resp;
}

LoopProjectFileResponse DataCollection::SetDataCollectionSources(netCDF::NcGroup* rootNode, DataCollectionSources sources, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("DataCollection") == groups.end()) {
            rootNode->addGroup("DataCollection");
        }
        netCDF::NcGroup dataCollectionGroup = rootNode->getGroup("DataCollection");
        dataCollectionGroup.putAtt("structureUrl",netCDF::ncChar,200,sources.structureUrl);
        dataCollectionGroup.putAtt("geologyUrl",netCDF::ncChar,200,sources.geologyUrl);
        dataCollectionGroup.putAtt("faultUrl",netCDF::ncChar,200,sources.faultUrl);
        dataCollectionGroup.putAtt("foldUrl",netCDF::ncChar,200,sources.foldUrl);
        dataCollectionGroup.putAtt("mindepUrl",netCDF::ncChar,200,sources.mindepUrl);
        dataCollectionGroup.putAtt("metadataUrl",netCDF::ncChar,200,sources.metadataUrl);
        dataCollectionGroup.putAtt("sourceTags",netCDF::ncChar,200,sources.sourceTags);
    } catch (netCDF::exceptions::NcException &e) {
        std::cout << e.what();
        resp = createErrorMsg(1,"Failed to add data collection configuration data to loop project file",verbose);
    }
    return resp;
}

} // namespace LoopProjectFile