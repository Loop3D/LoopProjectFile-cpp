#include "LoopVersion.h"

namespace LoopProjectFile {

LoopProjectFileResponse LoopVersion::SetVersion(netCDF::NcGroup* rootNode, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    if (!rootNode) {
        resp = createErrorMsg(1,"ERROR: Root Node invalid!");
        return resp;
    }
    try {
        rootNode->putAtt("loopMajorVersion",netCDF::ncInt64,LoopVersionMajor);
        rootNode->putAtt("loopMinorVersion",netCDF::ncInt64,LoopVersionMinor);
        rootNode->putAtt("loopSubVersion",netCDF::ncInt64,LoopVersionSub);
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) e.what();
        resp = createErrorMsg(1,"ERROR: Failure to set version on netCDF file", verbose);
    }
    return resp;
}

bool LoopVersion::CheckVersionValid(netCDF::NcGroup* rootNode, bool verbose)
{
    LoopVersion version;
    if (!rootNode->isNull()) {
        return false;
    }
    try {
        rootNode->getAtt("loopMajorVersion").getValues(&version.majorVersion);
        rootNode->getAtt("loopMinorVersion").getValues(&version.minorVersion);
        rootNode->getAtt("loopSubVersion").getValues(&version.subVersion);
        if (verbose) {
            std::cout << "  Loop Project File version = "
                << version.majorVersion << "." << version.minorVersion << "." << version.subVersion
                << std::endl;
        }
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what();
        return false;
    }
    return true;
}

LoopVersion LoopVersion::GetVersion(netCDF::NcGroup* rootNode, bool verbose)
{
    LoopVersion version;
    if (rootNode->isNull()) {
        return version;
    }
    try {
        rootNode->getAtt("loopMajorVersion").getValues(&version.majorVersion);
        rootNode->getAtt("loopMinorVersion").getValues(&version.minorVersion);
        rootNode->getAtt("loopSubVersion").getValues(&version.subVersion);
        version.errored = false;
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) {
            e.what();
            std::cout << "No valid Version in Loop Project File" << std::endl;
            version.errored = true;
        }
    }
    return version;
}

} // namespace LoopProjectFile