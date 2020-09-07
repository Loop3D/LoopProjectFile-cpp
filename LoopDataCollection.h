#ifndef __LOOPDATACOLLECTION_H
#define __LOOPDATACOLLECTION_H

#include <list>
#include <math.h>
#include "LoopProjectFileUtils.h"
#include <netcdf>

#define LOOP_FOLIATION_NAME_LENGTH 30
#define LOOP_WHAT_IS_FOLDED_NAME_LENGTH 30


namespace LoopProjectFile {

enum ObservationType {
    INVALIDOBSERVATION = -1,
    FAULTOBSERVATION,
    FOLDOBSERVATION,
    FOLIATIONOBSERVATION,
    DISCONTINUITYOBSERVATION,
    STRATIGRAPHICOBSERVATION,
    NUM_OBSERVATION_TYPES
};
/*! \brief A structure describing a generic observation */
struct Observation {
    int eventId; /*!< The event identifier of the event that this observation belongs to */
    /*! @{ The location of this observation in UTM coordinates (metres) */
    double easting;
    double northing;
    double altitude;
    /*!@}*/
    ObservationType type;
    Observation() {
        type = INVALIDOBSERVATION;
        eventId = 0;
        easting = 0;
        northing = 0;
        altitude = 0;
    }
};

/*! \brief A structure describing a single fault observation */
struct FaultObservation : public Observation {
    double dipdir; /*!< The compass direction of the observed fault dip (degrees) */
    double dip; /*!< The angle of the observed fault dip 0=horizontal 90=vertical down (degrees) */
    double dipPolarity; /*!< The dipPolarity of the dip 0=normal 1=inverted */
    double val; /*!< The calculated depth on the stratigraphic column of the observation (metres) */
    double displacement; /*!< The displacement of the fault at this observation */
    /*! Contructor. Zeros all variables */
    FaultObservation() : Observation() {
        type = FAULTOBSERVATION;
        dipdir = 0;
        dip = 0;
        dipPolarity = 0;
        val = 0;
        displacement = 0;
    }
};

/*! \brief A structure describing a single fold observation */
struct FoldObservation : public Observation {
    /*! @{ The 3D vector of the observed fold axis in XYZ (arbitrary units) */
    double axisX;
    double axisY;
    double axisZ;
    /*!@}*/
    char foliation[LOOP_FOLIATION_NAME_LENGTH]; /*!< The name of the most recent proceeding foliation event that is folded */
    char whatIsFolded[LOOP_WHAT_IS_FOLDED_NAME_LENGTH]; /*!< The name of the most recent proceeding fold event (if any) that is folded */
    /*! Contructor. Zeros all variables */ 
    FoldObservation() : Observation() {
        type = FOLDOBSERVATION;
        axisX = 0;
        axisY = 0;
        axisZ = 0;
        for (auto i=0;i<LOOP_FOLIATION_NAME_LENGTH;i++) foliation[i] = 0;
        for (auto i=0;i<LOOP_WHAT_IS_FOLDED_NAME_LENGTH;i++) whatIsFolded[i] = 0;
        whatIsFolded[0] = 0;
    }
};

/*! \brief A structure describing a single foliation observation */
struct FoliationObservation : public Observation {
    double dipdir; /*!< The compass direction of the observed foliation dip (degrees) */
    double dip; /*!< The angle of the observed foliation dip 0=horizontal 90=vertical down (degrees) */
    /*! Contructor. Zeros all variables */
    FoliationObservation() : Observation() {
        type = FOLIATIONOBSERVATION;
        dipdir = 0;
        dip = 0;
    }
};

/*! \brief A structure describing a single discontinuity observation */
struct DiscontinuityObservation : public Observation {
    double dipdir; /*!< The compass direction of the observed discontinuity dip (degrees) */
    double dip; /*!< The angle of the observed discontinuity dip 0=horizontal 90=vertical down (degrees) */
    /*! Contructor. Zeros all variables */
    DiscontinuityObservation() : Observation() {
        type = DISCONTINUITYOBSERVATION;
        eventId = 0;
        easting = 0;
        northing = 0;
        altitude = 0;
        dipdir = 0;
        dip = 0;
    }
};

/*! \brief A structure describing a single stratigraphic observation */
struct StratigraphicObservation : public Observation {
    double dipdir; /*!< The compass direction of the observed discontinuity dip (degrees) */
    double dip; /*!< The angle of the observed discontinuity dip 0=horizontal 90=vertical down (degrees) */
    double dipPolarity; /*!< The dipPolarity of the layer 0=normal 1=inverted */
    char layer[LOOP_NAME_LENGTH]; /*!< The name of the most recent stratigrapihc layer */
    /*! Contructor. Zeros all variables */
    StratigraphicObservation() : Observation() {
        type = STRATIGRAPHICOBSERVATION;
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
bool CheckDataCollectionValid(netCDF::NcGroup* rootNode, bool verbose=false);

/*!
 * \brief Creates the data collection structure in the given netCDF root node
 *
 * \param dataCollectionGroup - a pointer to the netCDF Group node "DataCollection"
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of structure creation with an error message if it failed
 */
LoopProjectFileResponse CreateObservationGroup(netCDF::NcGroup* dataCollectionGroup, bool verbose=false);

/*!
 * \brief Retrieves fault observation data from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - a reference to where the observation data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFaultObservations(netCDF::NcGroup* rootNode, std::vector<FaultObservation>& observations, bool verbose=false);

/*!
 * \brief Retrieves fold observation data from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - a reference to where the observation data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFoldObservations(netCDF::NcGroup* rootNode, std::vector<FoldObservation>& observations, bool verbose=false);

/*!
 * \brief Retrieves foliation observation data from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - a reference to where the observation data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetFoliationObservations(netCDF::NcGroup* rootNode, std::vector<FoliationObservation>& observations, bool verbose=false);

/*!
 * \brief Retrieves discontinuity observation data from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - a reference to where the observation data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetDiscontinuityObservations(netCDF::NcGroup* rootNode, std::vector<DiscontinuityObservation>& observations, bool verbose=false);

/*!
 * \brief Retrieves stratigrphic observation data from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - a reference to where the observation data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetStratigraphicObservations(netCDF::NcGroup* rootNode, std::vector<StratigraphicObservation>& observations, bool verbose=false);

/*!
 * \brief Sets fault observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetFaultObservations(netCDF::NcGroup* rootNode, std::vector<FaultObservation> observations, bool verbose=false);

/*!
 * \brief Sets fold observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoldObservations(netCDF::NcGroup* rootNode, std::vector<FoldObservation> observations, bool verbose=false);

/*!
 * \brief Sets foliation observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoliationObservations(netCDF::NcGroup* rootNode, std::vector<FoliationObservation> observations, bool verbose=false);

/*!
 * \brief Sets discontinuity observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetDiscontinuityObservations(netCDF::NcGroup* rootNode, std::vector<DiscontinuityObservation> observations, bool verbose=false);

/*!
 * \brief Sets stratigraphic observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data is to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetStratigraphicObservations(netCDF::NcGroup* rootNode, std::vector<StratigraphicObservation> observations, bool verbose=false);

} // namespace DataCollection
} // namespace LoopProjectFile

#endif