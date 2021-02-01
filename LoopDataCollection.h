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
        dipdir = 0;
        dip = 0;
    }
};

struct DataCollectionConfiguration {
    int orientationDecimate;
    int contactDecimate;
    int intrusionMode;
    int interpolationSpacing;
    int misorientation;
    char interpolationScheme[30];
    int faultDecimate;
    double minFaultLength;
    double faultDip;
    double plutonDip;
    char plutonForm[30];
    double distBuffer;
    double contactDip;
    int contactOrientationDecimate;
    char nullScheme[30];
    double thicknessBuffer;
    double maxThicknessAllowed;
    int foldDecimate;
    double fatStep;
    double closeDip;
    int useInterpolations;
    int useFat;
    DataCollectionConfiguration() {
        for (int i=0;i<30;i++) {
            interpolationScheme[i] = 0;
            plutonForm[i] = 0;
            nullScheme[i] = 0;
        }
        orientationDecimate = 0;
        contactDecimate = 5; 
        intrusionMode = 0;
        interpolationSpacing = 500;
        misorientation = 30;
        strncpy_s(interpolationScheme,"scipy_rbf",9);
        faultDecimate = 5;
        minFaultLength = 5000.;
        faultDip = 90.;
        plutonDip = 45.;
        strncpy_s(plutonForm,"domes",5);
        distBuffer = 10;
        contactDip = -999.;
        contactOrientationDecimate = 5;
        strncpy_s(nullScheme,"null",4);
        thicknessBuffer = 5000.;
        maxThicknessAllowed = 10000.;
        foldDecimate = 5;
        fatStep = 750.;
        closeDip = -999.;
        useInterpolations = 1;
        useFat = 1;
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
        dipdir = 0;
        dip = 0;
        dipPolarity = 0;
        for (auto i=0;i<LOOP_NAME_LENGTH;i++) layer[i] = 0;
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
 * \brief Retrieves stratigraphic observation data from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - a reference to where the observation data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetStratigraphicObservations(netCDF::NcGroup* rootNode, std::vector<StratigraphicObservation>& observations, bool verbose=false);

/*!
 * \brief Retrieves data collection configuration from the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param configuration - a reference to where the configuration data is to be copied 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data retrieval with an error message if it failed
 */
LoopProjectFileResponse GetDataCollectionConfiguration(netCDF::NcGroup* rootNode, DataCollectionConfiguration& configuration, bool verbose=false);

/*!
 * \brief Sets fault observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetFaultObservations(netCDF::NcGroup* rootNode, std::vector<FaultObservation> observations, bool verbose=false);

/*!
 * \brief Sets fold observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoldObservations(netCDF::NcGroup* rootNode, std::vector<FoldObservation> observations, bool verbose=false);

/*!
 * \brief Sets foliation observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetFoliationObservations(netCDF::NcGroup* rootNode, std::vector<FoliationObservation> observations, bool verbose=false);

/*!
 * \brief Sets discontinuity observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetDiscontinuityObservations(netCDF::NcGroup* rootNode, std::vector<DiscontinuityObservation> observations, bool verbose=false);

/*!
 * \brief Sets stratigraphic observation data to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param observations - the observation data to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetStratigraphicObservations(netCDF::NcGroup* rootNode, std::vector<StratigraphicObservation> observations, bool verbose=false);

/*!
 * \brief Sets data collection configuration to the loop project file
 *
 * \param rootNode - the rootNode of the netCDF Loop project file
 * \param configuration - the confifuration to be inserted 
 * \param verbose - a flag to toggle verbose message printing
 *
 * \return Response with success/fail of observation data insertion with an error message if it failed
 */
LoopProjectFileResponse SetDataCollectionConfiguration(netCDF::NcGroup* rootNode, DataCollectionConfiguration configuration, bool verbose=false);
} // namespace DataCollection
} // namespace LoopProjectFile

#endif