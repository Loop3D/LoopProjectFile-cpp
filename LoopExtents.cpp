#include "LoopExtents.h"

namespace LoopProjectFile {

LoopProjectFileResponse LoopExtents::SetExtents(netCDF::NcGroup* rootNode, LoopExtents extents, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    if (!rootNode) {
        resp = createErrorMsg(1,"ERROR: Group Node invalid!");
        return resp;
    }
    try {
        rootNode->putAtt("minLatitude",netCDF::ncDouble,extents.minLatitude);
        rootNode->putAtt("maxLatitude",netCDF::ncDouble,extents.maxLatitude);
        rootNode->putAtt("minLongitude",netCDF::ncDouble,extents.minLongitude);
        rootNode->putAtt("maxLongitude",netCDF::ncDouble,extents.maxLongitude);
        rootNode->putAtt("minNorthing",netCDF::ncDouble,extents.minNorthing);
        rootNode->putAtt("maxNorthing",netCDF::ncDouble,extents.maxNorthing);
        rootNode->putAtt("minEasting",netCDF::ncDouble,extents.minEasting);
        rootNode->putAtt("maxEasting",netCDF::ncDouble,extents.maxEasting);
        rootNode->putAtt("utmZone",netCDF::ncInt,extents.utmZone);
        rootNode->putAtt("utmNorthSouth",netCDF::ncInt,extents.utmNorthSouth);
        rootNode->putAtt("minDepth",netCDF::ncDouble,extents.minDepth);
        rootNode->putAtt("maxDepth",netCDF::ncDouble,extents.maxDepth);
        rootNode->putAtt("spacingX",netCDF::ncDouble,extents.spacingX);
        rootNode->putAtt("spacingY",netCDF::ncDouble,extents.spacingY);
        rootNode->putAtt("spacingZ",netCDF::ncDouble,extents.spacingZ);
        rootNode->putAtt("workingFormat",netCDF::ncInt,extents.workingFormat);
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what();
        resp = createErrorMsg(1,"ERROR: Failure to set extents on netCDF file", verbose);
    }
    return resp;
}

LoopProjectFileResponse LoopExtents::GetExtents(netCDF::NcGroup* rootNode, LoopExtents& extents, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    extents.errored = true;
    if (rootNode->isNull()) {
        return resp;
    }
    try {
        rootNode->getAtt("minLatitude").getValues(&extents.minLatitude);
        rootNode->getAtt("maxLatitude").getValues(&extents.maxLatitude);
        rootNode->getAtt("minLongitude").getValues(&extents.minLongitude);
        rootNode->getAtt("maxLongitude").getValues(&extents.maxLongitude);
        rootNode->getAtt("minNorthing").getValues(&extents.minNorthing);
        rootNode->getAtt("maxNorthing").getValues(&extents.maxNorthing);
        rootNode->getAtt("minEasting").getValues(&extents.minEasting);
        rootNode->getAtt("maxEasting").getValues(&extents.maxEasting);
        rootNode->getAtt("utmZone").getValues(&extents.utmZone);
        rootNode->getAtt("utmNorthSouth").getValues(&extents.utmNorthSouth);
        rootNode->getAtt("minDepth").getValues(&extents.minDepth);
        rootNode->getAtt("maxDepth").getValues(&extents.maxDepth);
        rootNode->getAtt("spacingX").getValues(&extents.spacingX);
        rootNode->getAtt("spacingY").getValues(&extents.spacingY);
        rootNode->getAtt("spacingZ").getValues(&extents.spacingZ);
        rootNode->getAtt("workingFormat").getValues(&extents.workingFormat);
        extents.errored = false;
    } catch (netCDF::exceptions::NcException& e) {
        resp = createErrorMsg(1,"No valid Extents in Loop Project File",verbose);
        if (verbose) std::cout << e.what() << std::endl;
    }
    return resp;
}

bool LoopExtents::CheckExtentsValid(netCDF::NcGroup* rootNode, std::vector<int>& xyzGridSize, bool verbose)
{
    bool valid = true;
    // Check Projection Model
    auto attributes = rootNode->getAtts();
    if (attributes.find("workingFormat") != attributes.end()) {
        if (verbose) {
            int workingFormat;
            rootNode->getAtt("workingFormat").getValues(&workingFormat);
            std::cout << "  Working in " << (workingFormat ? "Geodesic" : "UTM")
                << " Projection" << std::endl;
        }
    } else {
        std::cout << "(INVALID) No working format (Geodesic or UTM selection) in project file" << std::endl;
        valid = false;
    }

    // Check geodesic extents
    if (attributes.find("minLatitude") != attributes.end()
      && attributes.find("maxLatitude") != attributes.end()
      && attributes.find("minLongitude") != attributes.end()
      && attributes.find("maxLongitude") != attributes.end()) {
        double minLatitude, maxLatitude, minLongitude, maxLongitude;
        rootNode->getAtt("minLatitude").getValues(&minLatitude);
        rootNode->getAtt("maxLatitude").getValues(&maxLatitude);
        rootNode->getAtt("minLongitude").getValues(&minLongitude);
        rootNode->getAtt("maxLongitude").getValues(&maxLongitude);
        if (verbose) {
            std::cout << "  Geodesic extents found (deg)" << std::endl;
            std::cout << "\t minLatitude   = " << minLatitude << std::endl;
            std::cout << "\t maxLatitude   = " << maxLatitude << std::endl;
            std::cout << "\t minLongitude  = " << minLongitude << std::endl;
            std::cout << "\t maxLongitude  = " << maxLongitude << std::endl;
        }
    } else {
        std::cout << "(INVALID) No Geodesic extents found" << std::endl;
        valid = false;
    }

    // Check UTM extents
    double minNorthing, maxNorthing, minEasting, maxEasting;
    if (attributes.find("minNorthing") != attributes.end()
      && attributes.find("maxNorthing") != attributes.end()
      && attributes.find("minEasting") != attributes.end()
      && attributes.find("maxEasting") != attributes.end()
      && attributes.find("utmZone") != attributes.end()
      && attributes.find("utmNorthSouth") != attributes.end()) {
        int utmZone, utmNorthSouth;
        rootNode->getAtt("minNorthing").getValues(&minNorthing);
        rootNode->getAtt("maxNorthing").getValues(&maxNorthing);
        rootNode->getAtt("minEasting").getValues(&minEasting);
        rootNode->getAtt("maxEasting").getValues(&maxEasting);
        rootNode->getAtt("utmZone").getValues(&utmZone);
        rootNode->getAtt("utmNorthSouth").getValues(&utmNorthSouth);

        if (verbose) {
            std::cout << "  UTM extents found (m)" << std::endl;
            std::cout << "\t minNorthing   = " << minNorthing << std::endl;
            std::cout << "\t maxNorthing   = " << maxNorthing << std::endl;
            std::cout << "\t minEasting    = " << minEasting << std::endl;
            std::cout << "\t maxEasting    = " << maxEasting << std::endl;
            std::cout << "\t utmZone       = " << utmZone << std::endl;
            std::cout << "\t utmNorthSouth = " << utmNorthSouth << std::endl;
        }
    } else {
        std::cout << "(INVALID) No UTM extents found" << std::endl;
        valid = false;
    }

    // Check Depth extents
    double minDepth, maxDepth;
    if (attributes.find("minDepth") != attributes.end()
      && attributes.find("maxDepth") != attributes.end()) {
        rootNode->getAtt("minDepth").getValues(&minDepth);
        rootNode->getAtt("maxDepth").getValues(&maxDepth);
        if (verbose) {
            std::cout << "  Depth extents found (m)" << std::endl;
            std::cout << "\t minDepth     = " << minDepth << std::endl;
            std::cout << "\t maxDepth     = " << maxDepth << std::endl;

        }
    } else {
        std::cout << "(INVALID) No Depth extents found" << std::endl;
        valid = false;
    }

    // Check X/Y/Z spacing
    int spacingX, spacingY, spacingZ;
    if (attributes.find("spacingX") != attributes.end()
      && attributes.find("spacingY") != attributes.end()
      && attributes.find("spacingZ") != attributes.end()) {
        rootNode->getAtt("spacingX").getValues(&spacingX);
        rootNode->getAtt("spacingY").getValues(&spacingY);
        rootNode->getAtt("spacingZ").getValues(&spacingZ);
        if (verbose) {
            std::cout << "  Axis spacing found (m)" << std::endl;
            std::cout << "\t spacing X axis - " << spacingX << std::endl;
            std::cout << "\t spacing Y axis - " << spacingY << std::endl;
            std::cout << "\t spacing Z axis - " << spacingZ << std::endl;
        }
    } else {
        std::cout << "(INVALID) No spacing information in project file" << std::endl;
        valid = false;
    }

    if (valid) {
        xyzGridSize.clear();
        xyzGridSize.push_back((int)((maxEasting-minEasting)/((double)spacingX) + 1));
        xyzGridSize.push_back((int)((maxNorthing-minNorthing)/((double)spacingY) + 1));
        xyzGridSize.push_back((int)((maxDepth-minDepth)/((double)spacingZ) + 1));
    }
    return valid;
}

} // namespace LoopProjectFile