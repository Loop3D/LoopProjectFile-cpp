#ifndef __LOOPDATACOLLECTION_H
#define __LOOPDATACOLLECTION_H

#include <list>
#include <math.h>
#include "LoopProjectFileUtils.h"
#include <netcdf>

#define LOOP_FOLIATION_NAME_LENGTH 120
#define LOOP_WHAT_IS_FOLDED_NAME_LENGTH 120
#define LOOP_DRILLHOLE_PROPERTY_CODE_LENGTH 120
#define LOOP_DRILLHOLE_PROPERTY1_LENGTH 120
#define LOOP_DRILLHOLE_PROPERTY2_LENGTH 120
#define LOOP_DRILLHOLE_UNIT_LENGTH 120
#define LOOP_DRILLHOLE_PROPERTY_NAME_LENGTH 120
#define LOOP_DRILLHOLE_PROPERTY_VALUE_LENGTH 80
#define LOOP_DRILLHOLE_SURVEY_UNIT_LENGTH 80

namespace LoopProjectFile
{

    enum ObservationType
    {
        INVALIDOBSERVATION = -1,
        FAULTOBSERVATION,
        FOLDOBSERVATION,
        FOLIATIONOBSERVATION,
        DISCONTINUITYOBSERVATION,
        STRATIGRAPHICOBSERVATION,
        CONTACTOBSERVATION,
        DRILLHOLEOBSERVATION,
        NUM_OBSERVATION_TYPES
    };
    /*! \brief A structure describing a generic observation */
    struct Observation
    {
        int eventId; /*!< The event identifier of the event that this observation belongs to */
        /*! @{ The location of this observation in UTM coordinates (metres) */
        double easting;
        double northing;
        double altitude;
        /*!@}*/
        ObservationType type;
        Observation()
        {
            type = INVALIDOBSERVATION;
            eventId = 0;
            easting = 0;
            northing = 0;
            altitude = 0;
        }
    };

    /*! \brief A structure describing a single fault observation */
    struct FaultObservation : public Observation
    {
        double dipdir;       /*!< The compass direction of the observed fault dip (degrees) */
        double dip;          /*!< The angle of the observed fault dip 0=horizontal 90=vertical down (degrees) */
        double dipPolarity;  /*!< The dipPolarity of the dip 0=normal 1=inverted */
        double val;          /*!< The calculated depth on the stratigraphic column of the observation (metres) */
        double displacement; /*!< The displacement of the fault at this observation */
        char posOnly;        /*!< A flag to indicate that this observation is a position only */
        /*! Contructor. Zeros all variables */
        FaultObservation() : Observation()
        {
            type = FAULTOBSERVATION;
            dipdir = 0;
            dip = 0;
            dipPolarity = 0;
            val = 0;
            displacement = 0;
            posOnly = 0;
        }
    };

    /*! \brief A structure describing a single fold observation */
    struct FoldObservation : public Observation
    {
        /*! @{ The 3D vector of the observed fold axis in XYZ (arbitrary units) */
        double axisX;
        double axisY;
        double axisZ;
        /*!@}*/
        char foliation[LOOP_FOLIATION_NAME_LENGTH];         /*!< The name of the most recent proceeding foliation event that is folded */
        char whatIsFolded[LOOP_WHAT_IS_FOLDED_NAME_LENGTH]; /*!< The name of the most recent proceeding fold event (if any) that is folded */
        /*! Contructor. Zeros all variables */
        FoldObservation() : Observation()
        {
            type = FOLDOBSERVATION;
            axisX = 0;
            axisY = 0;
            axisZ = 0;
            for (auto i = 0; i < LOOP_FOLIATION_NAME_LENGTH; i++)
                foliation[i] = 0;
            for (auto i = 0; i < LOOP_WHAT_IS_FOLDED_NAME_LENGTH; i++)
                whatIsFolded[i] = 0;
            whatIsFolded[0] = 0;
        }
    };

    /*! \brief A structure describing a single foliation observation */
    struct FoliationObservation : public Observation
    {
        double dipdir; /*!< The compass direction of the observed foliation dip (degrees) */
        double dip;    /*!< The angle of the observed foliation dip 0=horizontal 90=vertical down (degrees) */
        /*! Contructor. Zeros all variables */
        FoliationObservation() : Observation()
        {
            type = FOLIATIONOBSERVATION;
            dipdir = 0;
            dip = 0;
        }
    };

    /*! \brief A structure describing a single discontinuity observation */
    struct DiscontinuityObservation : public Observation
    {
        double dipdir; /*!< The compass direction of the observed discontinuity dip (degrees) */
        double dip;    /*!< The angle of the observed discontinuity dip 0=horizontal 90=vertical down (degrees) */
        /*! Contructor. Zeros all variables */
        DiscontinuityObservation() : Observation()
        {
            type = DISCONTINUITYOBSERVATION;
            dipdir = 0;
            dip = 0;
        }
    };

    struct DataCollectionConfiguration
    {
        int quietMode;
        char deposits[LOOP_CONFIGURATION_DEFAULT_STRING_LENGTH];
        char dtb[LOOP_CONFIGURATION_DEFAULT_STRING_LENGTH];
        int orientationDecimate;
        int contactDecimate;
        int intrusionMode;
        int interpolationSpacing;
        int misorientation;
        char interpolationScheme[LOOP_CONFIGURATION_DEFAULT_STRING_LENGTH];
        int faultDecimate;
        double minFaultLength;
        double faultDip;
        double plutonDip;
        char plutonForm[LOOP_CONFIGURATION_DEFAULT_STRING_LENGTH];
        double distBuffer;
        double contactDip;
        int contactOrientationDecimate;
        char nullScheme[LOOP_CONFIGURATION_DEFAULT_STRING_LENGTH];
        double thicknessBuffer;
        double maxThicknessAllowed;
        int foldDecimate;
        double fatStep;
        double closeDip;
        int useInterpolations;
        int useFat;
        DataCollectionConfiguration()
        {
            for (int i = 0; i < LOOP_CONFIGURATION_DEFAULT_STRING_LENGTH; i++)
            {
                deposits[i] = 0;
                dtb[i] = 0;
                interpolationScheme[i] = 0;
                plutonForm[i] = 0;
                nullScheme[i] = 0;
            }
            quietMode = 0;
            strncpy_s(deposits, "Fe,Cu,Au,NONE", 13);
            orientationDecimate = 0;
            contactDecimate = 5;
            intrusionMode = 0;
            interpolationSpacing = 500;
            misorientation = 30;
            strncpy_s(interpolationScheme, "scipy_rbf", 9);
            faultDecimate = 5;
            minFaultLength = 5000.;
            faultDip = 90.;
            plutonDip = 45.;
            strncpy_s(plutonForm, "domes", 5);
            distBuffer = 10;
            contactDip = -999.;
            contactOrientationDecimate = 5;
            strncpy_s(nullScheme, "null", 4);
            thicknessBuffer = 5000.;
            maxThicknessAllowed = 10000.;
            foldDecimate = 5;
            fatStep = 750.;
            closeDip = -999.;
            useInterpolations = 1;
            useFat = 1;
        }
    };

    struct DataCollectionSources
    {
        char structureUrl[200];
        char geologyUrl[200];
        char faultUrl[200];
        char foldUrl[200];
        char mindepUrl[200];
        char metadataUrl[200];
        char sourceTags[200];
        DataCollectionSources()
        {
            for (int i = 0; i < 200; i++)
            {
                structureUrl[i] = 0;
                geologyUrl[i] = 0;
                faultUrl[i] = 0;
                foldUrl[i] = 0;
                mindepUrl[i] = 0;
                metadataUrl[i] = 0;
                sourceTags[i] = 0;
            }
        };
    };

    /*! \brief A structure describing a single stratigraphic observation */
    struct StratigraphicObservation : public Observation
    {
        double dipdir;                /*!< The compass direction of the observed discontinuity dip (degrees) */
        double dip;                   /*!< The angle of the observed discontinuity dip 0=horizontal 90=vertical down (degrees) */
        double dipPolarity;           /*!< The dipPolarity of the layer 0=normal 1=inverted */
        char layer[LOOP_NAME_LENGTH]; /*!< The name of the most recent stratigrapihc layer */
        /*! Contructor. Zeros all variables */
        StratigraphicObservation() : Observation()
        {
            type = STRATIGRAPHICOBSERVATION;
            dipdir = 0;
            dip = 0;
            dipPolarity = 0;
            for (auto i = 0; i < LOOP_NAME_LENGTH; i++)
                layer[i] = 0;
        }
    };

    /*! \brief A structure describing a single stratigraphic contact observation */
    struct ContactObservation : public Observation
    {
        ContactObservation() : Observation()
        {
            type = CONTACTOBSERVATION;
        }
    };

    /*! \brief A structure describing a single drillhole observation */
    struct DrillholeObservation : public Observation
    {
        double toEasting;
        double toNorthing;
        double toAltitude;
        double from;
        double to;
        char propertyCode[LOOP_DRILLHOLE_PROPERTY_CODE_LENGTH];
        char property1[LOOP_DRILLHOLE_PROPERTY1_LENGTH];
        char property2[LOOP_DRILLHOLE_PROPERTY2_LENGTH];
        char unit[LOOP_DRILLHOLE_UNIT_LENGTH];
        DrillholeObservation() : Observation()
        {
            type = DRILLHOLEOBSERVATION;
            toEasting = 0;
            toNorthing = 0;
            toAltitude = 0;
            from = 0;
            to = 0;
            for (auto i = 0; i < LOOP_DRILLHOLE_PROPERTY_CODE_LENGTH; i++)
                propertyCode[i] = 0;
            for (auto i = 0; i < LOOP_DRILLHOLE_PROPERTY1_LENGTH; i++)
                property1[i] = 0;
            for (auto i = 0; i < LOOP_DRILLHOLE_PROPERTY2_LENGTH; i++)
                property2[i] = 0;
            for (auto i = 0; i < LOOP_DRILLHOLE_UNIT_LENGTH; i++)
                unit[i] = 0;
        }
    };

    /*! \brief A structure describing a single drillhole property */
    struct DrillholeProperty
    {
        double collarId;
        char propertyName[LOOP_DRILLHOLE_PROPERTY_NAME_LENGTH];
        char propertyValue[LOOP_DRILLHOLE_PROPERTY_VALUE_LENGTH];
        DrillholeProperty()
        {
            collarId = 0;
            for (auto i = 0; i < LOOP_DRILLHOLE_PROPERTY_NAME_LENGTH; i++)
                propertyName[i] = 0;
            for (auto i = 0; i < LOOP_DRILLHOLE_PROPERTY_VALUE_LENGTH; i++)
                propertyValue[i] = 0;
        }
    };

    /*! \brief A structure describing a single drillhole survey observation */
    struct DrillholeSurvey
    {
        double collarId;
        double depth;
        double angle1;
        double angle2;
        char unit[LOOP_DRILLHOLE_SURVEY_UNIT_LENGTH];
        DrillholeSurvey()
        {
            collarId = 0;
            depth = 0;
            angle1 = 0;
            angle2 = 0;
            for (auto i = 0; i < LOOP_DRILLHOLE_SURVEY_UNIT_LENGTH; i++)
                unit[i] = 0;
        }
    };

    namespace DataCollection
    {

        /*!
         * \brief Checks the structure of the given netCDF root node for the appropriate
         * data collection structure
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return  A flag indicating whether the netCDF file is valid for data collection data
         */
        bool CheckDataCollectionValid(netCDF::NcGroup *rootNode, bool verbose = false);

        /*!
         * \brief Creates the data collection structure in the given netCDF root node
         *
         * \param dataCollectionGroup - a pointer to the netCDF Group node "DataCollection"
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of structure creation with an error message if it failed
         */
        LoopProjectFileResponse CreateObservationGroup(netCDF::NcGroup *dataCollectionGroup, bool verbose = false);

        /*!
         * \brief Creates the data collection contact structure in the given netCDF root node
         *
         * \param dataCollectionGroup - a pointer to the netCDF Group node "DataCollection"
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of structure creation with an error message if it failed
         */
        LoopProjectFileResponse CreateContactGroup(netCDF::NcGroup *dataCollectionGroup, bool verbose = false);

        /*!
         * \brief Creates the data collection drillhole structure in the given netCDF root node
         *
         * \param dataCollectionGroup - a pointer to the netCDF Group node "DataCollection"
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of structure creation with an error message if it failed
         */
        LoopProjectFileResponse CreateDrillholeGroup(netCDF::NcGroup *dataCollectionGroup, bool verbose = false);

        /*!
         * \brief Retrieves fault observation data from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - a reference to where the observation data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetFaultObservations(netCDF::NcGroup *rootNode, std::vector<FaultObservation> &observations, bool verbose = false);

        /*!
         * \brief Retrieves fold observation data from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - a reference to where the observation data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetFoldObservations(netCDF::NcGroup *rootNode, std::vector<FoldObservation> &observations, bool verbose = false);

        /*!
         * \brief Retrieves foliation observation data from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - a reference to where the observation data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetFoliationObservations(netCDF::NcGroup *rootNode, std::vector<FoliationObservation> &observations, bool verbose = false);

        /*!
         * \brief Retrieves discontinuity observation data from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - a reference to where the observation data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetDiscontinuityObservations(netCDF::NcGroup *rootNode, std::vector<DiscontinuityObservation> &observations, bool verbose = false);

        /*!
         * \brief Retrieves stratigraphic observation data from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - a reference to where the observation data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetStratigraphicObservations(netCDF::NcGroup *rootNode, std::vector<StratigraphicObservation> &observations, bool verbose = false);

        /*!
         * \brief Retrieves stratigraphic contact observation data from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - a reference to where the observation data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetContactObservations(netCDF::NcGroup *rootNode, std::vector<ContactObservation> &observations, bool verbose = false);

        /*!
         * \brief Retrieves drillhole observation data from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - a reference to where the observation data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetDrillholeObservations(netCDF::NcGroup *rootNode, std::vector<DrillholeObservation> &observations, bool verbose = false);

        /*!
         * \brief Retrieves drillhole property data from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param properties - a reference to where the propertu data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetDrillholeProperties(netCDF::NcGroup *rootNode, std::vector<DrillholeProperty> &properties, bool verbose = false);

        /*!
         * \brief Retrieves drillhole survey data from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param surveys - a reference to where the survey data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetDrillholeSurveys(netCDF::NcGroup *rootNode, std::vector<DrillholeSurvey> &surveys, bool verbose = false);

        /*!
         * \brief Retrieves data collection configuration from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param configuration - a reference to where the configuration data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetDataCollectionConfiguration(netCDF::NcGroup *rootNode, DataCollectionConfiguration &configuration, bool verbose = false);

        /*!
         * \brief Retrieves data collection url sources from the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param sources - a reference to where the url sources data is to be copied
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data retrieval with an error message if it failed
         */
        LoopProjectFileResponse GetDataCollectionSources(netCDF::NcGroup *rootNode, DataCollectionSources &sources, bool verbose = false);

        /*!
         * \brief Sets fault observation data to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - the observation data to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetFaultObservations(netCDF::NcGroup *rootNode, std::vector<FaultObservation> observations, bool verbose = false);

        /*!
         * \brief Sets fold observation data to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - the observation data to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetFoldObservations(netCDF::NcGroup *rootNode, std::vector<FoldObservation> observations, bool verbose = false);

        /*!
         * \brief Sets foliation observation data to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - the observation data to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetFoliationObservations(netCDF::NcGroup *rootNode, std::vector<FoliationObservation> observations, bool verbose = false);

        /*!
         * \brief Sets discontinuity observation data to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - the observation data to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetDiscontinuityObservations(netCDF::NcGroup *rootNode, std::vector<DiscontinuityObservation> observations, bool verbose = false);

        /*!
         * \brief Sets stratigraphic observation data to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - the observation data to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetStratigraphicObservations(netCDF::NcGroup *rootNode, std::vector<StratigraphicObservation> observations, bool verbose = false);

        /*!
         * \brief Sets stratigraphic contact observation data to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - the observation data to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetContactObservations(netCDF::NcGroup *rootNode, std::vector<ContactObservation> observations, bool verbose = false);

        /*!
         * \brief Sets drillhole observation data to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param observations - the observation data to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetDrillholeObservations(netCDF::NcGroup *rootNode, std::vector<DrillholeObservation> observations, bool verbose = false);

        /*!
         * \brief Sets drillhole property data to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param properties - the property data to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetDrillholeProperties(netCDF::NcGroup *rootNode, std::vector<DrillholeProperty> properties, bool verbose = false);

        /*!
         * \brief Sets drillhole survey data to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param surveys - the survey data to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetDrillholeSurveys(netCDF::NcGroup *rootNode, std::vector<DrillholeSurvey> surveys, bool verbose = false);

        /*!
         * \brief Sets data collection configuration to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param configuration - the configuration to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetDataCollectionConfiguration(netCDF::NcGroup *rootNode, DataCollectionConfiguration configuration, bool verbose = false);

        /*!
         * \brief Sets data collection url sources to the loop project file
         *
         * \param rootNode - the rootNode of the netCDF Loop project file
         * \param sources - the url sources data to be inserted
         * \param verbose - a flag to toggle verbose message printing
         *
         * \return Response with success/fail of observation data insertion with an error message if it failed
         */
        LoopProjectFileResponse SetDataCollectionSources(netCDF::NcGroup *rootNode, DataCollectionSources sources, bool verbose = false);
    } // namespace DataCollection
} // namespace LoopProjectFile

#endif