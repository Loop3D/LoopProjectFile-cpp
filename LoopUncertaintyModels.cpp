#include "LoopUncertaintyModels.h"
#include "LoopExtents.h"

namespace LoopProjectFile {

bool UncertaintyModels::CheckUncertaintyModelsValid(netCDF::NcGroup* rootNode, std::vector<int> xyzGridSize, bool verbose)
{
    bool valid = true;
    auto groups = rootNode->getGroups();
    if (groups.find("UncertaintyModels") != groups.end()) {
        if (verbose) std::cout << "  Uncertainty Models Group Present" << std::endl;
        netCDF::NcGroup umGroup = rootNode->getGroup("UncertaintyModels");
        try {
            netCDF::NcDim easting = umGroup.getDim("easting");
            netCDF::NcDim northing = umGroup.getDim("northing");
            netCDF::NcDim depth = umGroup.getDim("depth");
            if (xyzGridSize.size() == 3 && !easting.isNull() && !northing.isNull() && !depth.isNull()) {
                if (easting.getSize() != xyzGridSize[0]
                    || northing.getSize() != xyzGridSize[1]
                    || depth.getSize() != xyzGridSize[2]) {
                        if (verbose) {
                            std::cout << "(INVALID) Extents grid size and Uncertainty Models Grid Size do NOT match" << std::endl;
                            std::cout << "(INVALID) Extents Grid Size : " << xyzGridSize[0] << " " << xyzGridSize[1] << " " << xyzGridSize[2] << std::endl;
                            std::cout << "(INVALID) Uncertainty Models Grid Size : " << easting.getSize() << " " << northing.getSize() << " " << depth.getSize() << std::endl;
                        }
                        valid = false;
                } else {
                    if (verbose) std::cout << "  Uncertainty Models grid size adheres to extents" << std::endl;
                }
            }
        } catch (netCDF::exceptions::NcException& e) {
            std::cout << e.what();
            valid = false;
        }
    } else {
        if (verbose) std::cout << "No Uncertainty Models Group Present" << std::endl;
    }
    return valid;
}

LoopProjectFileResponse UncertaintyModels::SetUncertaintyModel(netCDF::NcGroup* rootNode, std::vector<float> data, std::vector<int> dataShape, unsigned int index, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    std::vector<int> extents;
    LoopExtents::CheckExtentsValid(rootNode,extents,verbose);
    if (extents.size() != 3 || dataShape.size() != 3) {
        resp = createErrorMsg(1,"Trying to add Uncertainty Model without valid project file extents",verbose);
    } else {
        try {
            auto groups = rootNode->getGroups();
            if (groups.find("UncertaintyModels") == groups.end()) {
                netCDF::NcGroup umGroup = rootNode->addGroup("UncertaintyModels");
                netCDF::NcDim eastingDim = umGroup.addDim("easting",extents[0]);
                netCDF::NcDim northingDim = umGroup.addDim("northing",extents[1]);
                netCDF::NcDim depthDim = umGroup.addDim("depth",extents[2]);
                netCDF::NcDim indexDim = umGroup.addDim("index");
                std::vector<netCDF::NcDim> dims;
                dims.push_back(eastingDim);
                dims.push_back(northingDim);
                dims.push_back(depthDim);
                dims.push_back(indexDim);
                netCDF::NcVar dataVar = umGroup.addVar("data",netCDF::ncFloat,dims);
                dims.clear();
                dims.push_back(indexDim);
                netCDF::NcVar minValVar = umGroup.addVar("minVal",netCDF::ncFloat,dims);
                netCDF::NcVar maxValVar = umGroup.addVar("maxVal",netCDF::ncFloat,dims);
                minValVar.setEndianness(netCDF::NcVar::EndianMode::nc_ENDIAN_LITTLE);
                maxValVar.setEndianness(netCDF::NcVar::EndianMode::nc_ENDIAN_LITTLE);
                dataVar.setEndianness(netCDF::NcVar::EndianMode::nc_ENDIAN_LITTLE);
            }
            netCDF::NcGroup umGroup = rootNode->getGroup("UncertaintyModels");
            // Check data shape against extents shape
            for (auto i=0;i<3;i++) {
                if (dataShape[i] != extents[i]) {
                    std::cout << dataShape[0] << " " << dataShape[1] << " " << dataShape[2] << std::endl;
                    std::cout << extents[0] << " " << extents[1] << " " << extents[2] << std::endl;
                    return createErrorMsg(1,"Incoming Data Shape does not match project file datashape",verbose);
                }
            }

            // Check incoming data fits data shape
            if (data.size() != dataShape[0] * dataShape[1] * dataShape[2]) {
                return createErrorMsg(1,"Incoming data does not match incoming data shape",verbose);
            }

            // Add data to project file
            float min = NC_MAX_FLOAT;
            float max = NC_MIN_FLOAT;
            for (auto i=0;i<data.size();i++) {
                if (data[i] > max) max = data[i];
                if (data[i] < min) min = data[i];
            }
            std::vector<size_t> start; start.push_back(index);
            std::vector<size_t> count; count.push_back(1);
            netCDF::NcVar minValVar = umGroup.getVar("minVal");
            netCDF::NcVar maxValVar = umGroup.getVar("maxVal");
            netCDF::NcVar dataVar = umGroup.getVar("data");
            minValVar.putVar(start,count,&min);
            maxValVar.putVar(start,count,&max);
            start.clear();
            start.push_back(0); start.push_back(0); start.push_back(0); start.push_back(index);
            count.clear();
            count.push_back(dataShape[0]); count.push_back(dataShape[1]); count.push_back(dataShape[2]); count.push_back(1);
            dataVar.putVar(start,count,&data[0]);
        } catch (netCDF::exceptions::NcException& e) {
            if (verbose) std::cout << e.what() << std::endl;
            resp = createErrorMsg(1, "Failed to add Uncertainty Model to loop project file",verbose);
        }
    }
    return resp;
}

LoopProjectFileResponse UncertaintyModels::GetUncertaintyModel(netCDF::NcGroup *rootNode, std::vector<float> &data, std::vector<int> &dataShape, unsigned int index, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("UncertaintyModels") == groups.end()) {
            resp = createErrorMsg(1,"No Uncertainty Models Group in project file",verbose);
        } else {
            netCDF::NcGroup umGroup = rootNode->getGroup("UncertaintyModels");
            netCDF::NcDim easting = umGroup.getDim("easting");
            netCDF::NcDim northing = umGroup.getDim("northing");
            netCDF::NcDim depth = umGroup.getDim("depth");
            dataShape.clear();
            dataShape.push_back(static_cast<int>(easting.getSize()));
            dataShape.push_back(static_cast<int>(northing.getSize()));
            dataShape.push_back(static_cast<int>(depth.getSize()));
            data.clear();
            data.resize(dataShape[0]*dataShape[1]*dataShape[2]);
            std::vector<size_t> start;
            std::vector<size_t> count;
            netCDF::NcVar dataVar = umGroup.getVar("data");
            start.push_back(0); start.push_back(0); start.push_back(0); start.push_back(index);
            count.push_back(dataShape[0]); count.push_back(dataShape[1]); count.push_back(dataShape[2]); count.push_back(1);
            dataVar.getVar(start,count,&data[0]);
        }
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what() << std::endl;
        resp = createErrorMsg(1, "Failed to add Uncertainty Model to loop project file",verbose);
    }
    return resp;
}

} // namespace LoopProjectFile