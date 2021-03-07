#ifndef __LOOPEXTRACTEDINFORMATION_H
#define __LOOPEXTRACTEDINFORMATION_H

#include <netcdf>
#include "LoopProjectFileUtils.h"

namespace LoopProjectFile {

enum EventType {
    INVALIDEVENT = -1,
    FAULTEVENT,
    FOLDEVENT,
    FOLIATIONEVENT,
    DISCONTINUITYEVENT,
    STRATIGRAPHICLAYER,
    NUM_EVENT_TYPES
};

/*! \brief A structure describing a generic event */
struct Event {
    int eventId; /*!< A unique event identifier (unique to all events types) */
    /*!@{ Limits on the age of the event (arbitrary units) */
    double minAge;
    double maxAge;
    /*!@}*/
    char name[LOOP_NAME_LENGTH]; /*!< The name of this event */
    char supergroup[LOOP_SUPERGROUP_NAME_LENGTH]; /*!< The name of this event's supergroup */
    char enabled; /*!< A flag to enable/disable this event from modelling calculations */
    int rank; /*!< A placeholder for permutation calculations */
    EventType type;
    /*! Constructor. Zeros all values */
    Event() {
        type = INVALIDEVENT;
        eventId = 0;
        minAge = 0;
        maxAge = 0;
        enabled = 0;
        for (auto i=0;i<LOOP_NAME_LENGTH;i++) name[i] = 0;
        for (auto i=0;i<LOOP_SUPERGROUP_NAME_LENGTH;i++) supergroup[i] = 0;
    }
};

/*! \brief A structure describing a fault event */
struct FaultEvent : public Event {
    double avgDisplacement; /*!< The calculated average displacement across the fault (metres) */
    double avgDownthrowDir; /*!< The calculated average downthrow direction of the fault (degrees) */
    double influenceDistance; /*!< The calculated influence distance the fault (metres) */
    double verticalRadius; /*!< The calculated influence depth of the fault (metres) */
    double horizontalRadius; /*!< The calculated horizontal influence of the fault (metres) */
    char colour[7];
    /*! Constructor. Zeros all values */
    FaultEvent() : Event() {
        type = FAULTEVENT;
        avgDisplacement = 0;
        avgDownthrowDir = 0;
        influenceDistance = 0;
        verticalRadius = 0;
        horizontalRadius = 0;
        for (int i=0;i<7;i++) colour[i] = 0;
    }
};

/*! \brief A structure describing a fold event */
struct FoldEvent : public Event {
    char periodic; /*!< A flag indicating whether the fold is repeated */
    double wavelength; /*!< The wavelength of the fold (metres) */
    double amplitude; /*!< The amplitude of the fold (metres) */
    char asymmetry; /*!< A flag indicating whether the fold is asymmetrical */
    double asymmetryShift; /*!< The ratio of the asymmetry */
    double secondaryWavelength; /*!< The wavelength of the parasitic fold (metres) */
    double secondaryAmplitude; /*!< The amplitude of the parasitic fold (metres) */
    /*! Constructor. Zeros all values */
    FoldEvent() : Event() {
        type = FOLDEVENT;
        periodic = 0;
        wavelength = 0;
        amplitude = 0;
        asymmetry = 0;
        asymmetryShift = 0;
        secondaryWavelength = 0;
        secondaryAmplitude = 0;
    }
};

/*! \brief A structure describing a foliation event */
struct FoliationEvent : public Event {
    /*!@{ The limits of the foliation event in depth on the stratigraphic log */
    double lowerScalarValue;
    double upperScalarValue;
    /*!@}*/
    /*! Constructor. Zeros all values */
    FoliationEvent() : Event() {
        type = FOLIATIONEVENT;
        lowerScalarValue = 0;
        upperScalarValue = 0;
    }
};

/*! \brief A structure describing a discontinuity event */
struct DiscontinuityEvent : public Event {
    double scalarValue; /*!< The depth of the discontinuity event on the stratigraphic log */
    /*! Constructor. Zeros all values */
    DiscontinuityEvent() : Event() {
        type = DISCONTINUITYEVENT;
        scalarValue = 0;
    }
};

/*! \brief A structure describing a stratigraphic layer */
struct StratigraphicLayer : public Event {
    double thickness; /*!< The average thickness of the layer */
    /*!@{ RGB colour values for the top and bottom of the layer */
    char colour1Red;
    char colour1Green;
    char colour1Blue;
    char colour2Red;
    char colour2Green;
    char colour2Blue;
    /*!@}*/

    /*! Constructor. Zeros all values */
    StratigraphicLayer() : Event() {
        type = STRATIGRAPHICLAYER;
        thickness = 0;
        colour1Red = colour1Green = colour1Blue = 0;
        colour2Red = colour2Green = colour2Blue = 0;
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
bool CheckExtractedInformationValid(netCDF::NcGroup* rootNode, bool verbose=false);

/*!
 * \brief Creates the extracted information structure in the given netCDF root node
 *
 * \param extractedInformationGroup - a pointer to the netCDF Group node "ExtractedInformation"
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of structure creation with an error message if it failed
 */
LoopProjectFileResponse CreateExtractedInformationGroup(netCDF::NcGroup* extractedInformationGroup, bool verbose=false);

/*!
 * \brief Retrieves fault event information from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - a reference to where the event data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFaultEvents(netCDF::NcGroup* rootNode, std::vector<FaultEvent>& events, bool verbose=false);

/*!
 * \brief Retrieves fold event information from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - a reference to where the event data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFoldEvents(netCDF::NcGroup* rootNode, std::vector<FoldEvent>& events, bool verbose=false);

/*!
 * \brief Retrieves foliation event information from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - a reference to where the event data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFoliationEvents(netCDF::NcGroup* rootNode, std::vector<FoliationEvent>& events, bool verbose=false);

/*!
 * \brief Retrieves discontinuity event information from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - a reference to where the event data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information retrieval with an error message if it failed
 */
LoopProjectFileResponse GetDiscontinuityEvents(netCDF::NcGroup* rootNode, std::vector<DiscontinuityEvent>& events, bool verbose=false);

/*!
 * \brief Retrieves stratigraphic layer information from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param layers - a reference to where the layer data is to be copied
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information retrieval with an error message if it failed
 */
LoopProjectFileResponse GetStratigraphicLayers(netCDF::NcGroup* rootNode, std::vector<StratigraphicLayer>& layers, bool verbose=false);

/*!
 * \brief Sets fault event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetFaultEvents(netCDF::NcGroup* rootNode, std::vector<FaultEvent> events, bool verbose=false);

/*!
 * \brief Sets fold event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoldEvents(netCDF::NcGroup* rootNode, std::vector<FoldEvent> events, bool verbose=false);

/*!
 * \brief Sets foliation event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoliationEvents(netCDF::NcGroup* rootNode, std::vector<FoliationEvent> events, bool verbose=false);

/*!
 * \brief Sets discontinuity event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetDiscontinuityEvents(netCDF::NcGroup* rootNode, std::vector<DiscontinuityEvent> events, bool verbose=false);

/*!
 * \brief Sets stratigraphic log information in the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param layers - the stratigraphic layer data to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetStratigraphicLayers(netCDF::NcGroup* rootNode, std::vector<StratigraphicLayer> layers, bool verbose=false);

} // namespace ExtractedInformation
} // namespace LoopProjectFile

#endif