#ifndef __LOOPEXTRACTEDINFORMATION_H
#define __LOOPEXTRACTEDINFORMATION_H

#include <netcdf>
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
    /*!@{ The limits of the foliation event in depth on the stratigraphic log */
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
    double scalarValue; /*!< The depth of the discontinuity event on the stratigraphic log */
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
 * \brief Sets fault event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetFaultEvents(netCDF::NcGroup* rootNode, std::vector<FaultEvent> events, bool verbose=false);

/*!
 * \brief Sets fold event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoldEvents(netCDF::NcGroup* rootNode, std::vector<FoldEvent> events, bool verbose=false);

/*!
 * \brief Sets foliation event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoliationEvents(netCDF::NcGroup* rootNode, std::vector<FoliationEvent> events, bool verbose=false);

/*!
 * \brief Sets discontinuity event information to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param events - the event data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of event information insertion with an error message if it failed
 */
LoopProjectFileResponse SetDiscontinuityEvents(netCDF::NcGroup* rootNode, std::vector<DiscontinuityEvent> events, bool verbose=false);

} // namespace ExtractedInformation
} // namespace LoopProjectFile

#endif