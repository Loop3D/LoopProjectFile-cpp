#include "LoopStructuralModels.h"
#include "LoopExtents.h"

namespace LoopProjectFile {

bool StructuralModels::CheckStructuralModelsValid(netCDF::NcGroup* rootNode, std::vector<int> xyzGridSize, bool verbose)
{
    bool valid = true;
    auto groups = rootNode->getGroups();
    if (groups.find("StructuralModels") != groups.end()) {
        if (verbose) std::cout << "  Structural Models Group Present" << std::endl;
        netCDF::NcGroup smGroup = rootNode->getGroup("StructuralModels");
        try {
            netCDF::NcDim easting = smGroup.getDim("easting");
            netCDF::NcDim northing = smGroup.getDim("northing");
            netCDF::NcDim depth = smGroup.getDim("depth");
            if (xyzGridSize.size() == 3 && !easting.isNull() && !northing.isNull() && !depth.isNull()) {
                if (easting.getSize() != xyzGridSize[0]
                    || northing.getSize() != xyzGridSize[1]
                    || depth.getSize() != xyzGridSize[2]) {
                        if (verbose) {
                            std::cout << "(INVALID) Extents grid size and Structural Models Grid Size do NOT match" << std::endl;
                            std::cout << "(INVALID) Extents Grid Size : " << xyzGridSize[0] << " " << xyzGridSize[1] << " " << xyzGridSize[2] << std::endl;
                            std::cout << "(INVALID) Structural Models Grid Size : " << easting.getSize() << " " << northing.getSize() << " " << depth.getSize() << std::endl;
                        }
                        valid = false;
                } else {
                    if (verbose) std::cout << "  Structural Models grid size adheres to extents" << std::endl;
                }
            }
        } catch (netCDF::exceptions::NcException& e) {
            std::cout << e.what();
            valid = false;
        }
    } else {
        if (verbose) std::cout << "No Structural Models Group Present" << std::endl;
    }
    return valid;
}

LoopProjectFileResponse StructuralModels::SetStructuralModel(netCDF::NcGroup* rootNode, std::vector<float> data, std::vector<int> dataShape, unsigned int index, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    std::vector<int> extents;
    LoopExtents::CheckExtentsValid(rootNode,extents,verbose);
    if (extents.size() != 3 || dataShape.size() != 3) {
        resp = createErrorMsg(1,"Trying to add structural model without valid project file extents",verbose);
    } else {
        try {
            auto groups = rootNode->getGroups();
            if (groups.find("StructuralModels") == groups.end()) {
                netCDF::NcGroup smGroup = rootNode->addGroup("StructuralModels");
                netCDF::NcDim eastingDim = smGroup.addDim("easting",extents[0]);
                netCDF::NcDim northingDim = smGroup.addDim("northing",extents[1]);
                netCDF::NcDim depthDim = smGroup.addDim("depth",extents[2]);
                netCDF::NcDim indexDim = smGroup.addDim("index");
                std::vector<netCDF::NcDim> dims;
                dims.push_back(eastingDim);
                dims.push_back(northingDim);
                dims.push_back(depthDim);
                dims.push_back(indexDim);
                netCDF::NcVar dataVar = smGroup.addVar("data",netCDF::ncFloat,dims);
                dims.clear();
                dims.push_back(indexDim);
                netCDF::NcVar minValVar = smGroup.addVar("minVal",netCDF::ncFloat,dims);
                netCDF::NcVar maxValVar = smGroup.addVar("maxVal",netCDF::ncFloat,dims);
                netCDF::NcVar validVar = smGroup.addVar("valid",netCDF::ncChar,dims);
                minValVar.setEndianness(netCDF::NcVar::EndianMode::nc_ENDIAN_LITTLE);
                maxValVar.setEndianness(netCDF::NcVar::EndianMode::nc_ENDIAN_LITTLE);
                dataVar.setEndianness(netCDF::NcVar::EndianMode::nc_ENDIAN_LITTLE);
                validVar.setEndianness(netCDF::NcVar::EndianMode::nc_ENDIAN_LITTLE);
                char valid = 0;
                validVar.setFill(true,valid);
            }
            netCDF::NcGroup smGroup = rootNode->getGroup("StructuralModels");
            // Check data shape against extents shape
            for (auto i=0;i<3;i++) {
                if (dataShape[i] != extents[i]) {
                    std::cout << dataShape[0] << " " << dataShape[1] << " " << dataShape[2] << std::endl;
                    std::cout << extents[0] << " " << extents[1] << " " << extents[2] << std::endl;
                    return createErrorMsg(1,"Incoming Data Shape does not match project file datashape",verbose);
                }
            }

            // Check incoming data fits incoming data shape
            if (data.size() != dataShape[0] * dataShape[1] * dataShape[2]) {
                return createErrorMsg(1,"Incoming data does not match incoming data shape",verbose);
            }

            // Add data to project file
            float min = NC_MAX_FLOAT;
            float max = NC_MIN_FLOAT;
            char valid = 1;
            for (auto i=0;i<data.size();i++) {
                if (data[i] > max) max = data[i];
                if (data[i] < min) min = data[i];
            }
            std::vector<size_t> start; start.push_back(index);
            std::vector<size_t> count; count.push_back(1);
            netCDF::NcVar minValVar = smGroup.getVar("minVal");
            netCDF::NcVar maxValVar = smGroup.getVar("maxVal");
            netCDF::NcVar dataVar = smGroup.getVar("data");
            netCDF::NcVar validVar = smGroup.getVar("valid");
            minValVar.putVar(start,count,&min);
            maxValVar.putVar(start,count,&max);
            validVar.putVar(start,count,&valid);
            start.clear();
            start.push_back(0); start.push_back(0); start.push_back(0); start.push_back(index);
            count.clear();
            count.push_back(dataShape[0]); count.push_back(dataShape[1]); count.push_back(dataShape[2]); count.push_back(1);
            dataVar.putVar(start,count,&data[0]);
        } catch (netCDF::exceptions::NcException& e) {
            if (verbose) std::cout << e.what() << std::endl;
            resp = createErrorMsg(1, "Failed to add structural model to loop project file",verbose);
        }
    }
    return resp;
}

LoopProjectFileResponse StructuralModels::GetStructuralModel(netCDF::NcGroup* rootNode, std::vector<float>& data, std::vector<int>& dataShape, unsigned int index, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("StructuralModels") == groups.end()) {
            resp = createErrorMsg(1,"No Structural Models Group in project file",verbose);
        } else {
            netCDF::NcGroup smGroup = rootNode->getGroup("StructuralModels");
            // Check data is in that index location
            // ???How to check for data without going to an exception???
            netCDF::NcDim indexDim = smGroup.getDim("index");

            netCDF::NcDim easting = smGroup.getDim("easting");
            netCDF::NcDim northing = smGroup.getDim("northing");
            netCDF::NcDim depth = smGroup.getDim("depth");
            dataShape.clear();
            dataShape.push_back(easting.getSize());
            dataShape.push_back(northing.getSize());
            dataShape.push_back(depth.getSize());
            data.clear();
            data.resize(dataShape[0]*dataShape[1]*dataShape[2]);
            std::vector<size_t> start;
            std::vector<size_t> count;
            netCDF::NcVar dataVar = smGroup.getVar("data");
            start.push_back(0); start.push_back(0); start.push_back(0); start.push_back(index);
            count.push_back(dataShape[0]); count.push_back(dataShape[1]); count.push_back(dataShape[2]); count.push_back(1);
            dataVar.getVar(start,count,&data[0]);
        }
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what() << std::endl;
        resp = createErrorMsg(1, "Failed to add structural model to loop project file",verbose);
    }
    return resp;
}

} // namespace LoopProjectFile