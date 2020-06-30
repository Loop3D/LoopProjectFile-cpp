#include "LoopGeophysicalModels.h"
#include "LoopExtents.h"

namespace LoopProjectFile {

bool GeophysicalModels::CheckGeophysicalModelsValid(netCDF::NcGroup* rootNode, std::vector<int> xyzGridSize, bool verbose)
{
    bool valid = true;
    auto groups = rootNode->getGroups();
    if (groups.find("GeophysicalModels") != groups.end()) {
        if (verbose) std::cout << "  Geophysical Models Group Present" << std::endl;
        netCDF::NcGroup gmGroup = rootNode->getGroup("GeophysicalModels");
        try {
            netCDF::NcDim easting = gmGroup.getDim("easting");
            netCDF::NcDim northing = gmGroup.getDim("northing");
            netCDF::NcDim depth = gmGroup.getDim("depth");
            if (xyzGridSize.size() == 3 && !easting.isNull() && !northing.isNull() && !depth.isNull()) {
                if (easting.getSize() != xyzGridSize[0]
                    || northing.getSize() != xyzGridSize[1]
                    || depth.getSize() != xyzGridSize[2]) {
                        if (verbose) {
                            std::cout << "(INVALID) Extents grid size and Geophysical Models Grid Size do NOT match" << std::endl;
                            std::cout << "(INVALID) Extents Grid Size : " << xyzGridSize[0] << " " << xyzGridSize[1] << " " << xyzGridSize[2] << std::endl;
                            std::cout << "(INVALID) Geophysical Models Grid Size : " << easting.getSize() << " " << northing.getSize() << " " << depth.getSize() << std::endl;
                        }
                        valid = false;
                } else {
                    if (verbose) std::cout << "  Geophysical Models grid size adheres to extents" << std::endl;
                }
            }
        } catch (netCDF::exceptions::NcException& e) {
            std::cout << e.what();
            valid = false;
        }
    } else {
        if (verbose) std::cout << "No Geophysical Models Group Present" << std::endl;
    }
    return valid;
}

LoopProjectFileResponse GeophysicalModels::SetGeophysicalModel(netCDF::NcGroup* rootNode, std::vector<float> data, std::vector<int> dataShape, unsigned int index, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    std::vector<int> extents;
    LoopExtents::CheckExtentsValid(rootNode,extents,verbose);
    if (extents.size() != 3 || dataShape.size() != 3) {
        resp = createErrorMsg(1,"Trying to add Geophysical Model without valid project file extents",verbose);
    } else {
        try {
            auto groups = rootNode->getGroups();
            if (groups.find("GeophysicalModels") == groups.end()) {
                netCDF::NcGroup gmGroup = rootNode->addGroup("GeophysicalModels");
                netCDF::NcDim eastingDim = gmGroup.addDim("easting",extents[0]);
                netCDF::NcDim northingDim = gmGroup.addDim("northing",extents[1]);
                netCDF::NcDim depthDim = gmGroup.addDim("depth",extents[2]);
                netCDF::NcDim indexDim = gmGroup.addDim("index");
                std::vector<netCDF::NcDim> dims;
                dims.push_back(eastingDim);
                dims.push_back(northingDim);
                dims.push_back(depthDim);
                dims.push_back(indexDim);
                netCDF::NcVar dataVar = gmGroup.addVar("data",netCDF::ncFloat,dims);
                dims.clear();
                dims.push_back(indexDim);
                netCDF::NcVar minValVar = gmGroup.addVar("minVal",netCDF::ncFloat,dims);
                netCDF::NcVar maxValVar = gmGroup.addVar("maxVal",netCDF::ncFloat,dims);
                minValVar.setEndianness(netCDF::NcVar::EndianMode::nc_ENDIAN_LITTLE);
                maxValVar.setEndianness(netCDF::NcVar::EndianMode::nc_ENDIAN_LITTLE);
                dataVar.setEndianness(netCDF::NcVar::EndianMode::nc_ENDIAN_LITTLE);
            }
            netCDF::NcGroup gmGroup = rootNode->getGroup("GeophysicalModels");
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
            for (auto i=0;i<data.size();i++) {
                if (data[i] > max) max = data[i];
                if (data[i] < min) min = data[i];
            }
            std::vector<size_t> start; start.push_back(index);
            std::vector<size_t> count; count.push_back(1);
            netCDF::NcVar minValVar = gmGroup.getVar("minVal");
            netCDF::NcVar maxValVar = gmGroup.getVar("maxVal");
            netCDF::NcVar dataVar = gmGroup.getVar("data");
            minValVar.putVar(start,count,&min);
            maxValVar.putVar(start,count,&max);
            start.clear();
            start.push_back(0); start.push_back(0); start.push_back(0); start.push_back(index);
            count.clear();
            count.push_back(dataShape[0]); count.push_back(dataShape[1]); count.push_back(dataShape[2]); count.push_back(1);
            dataVar.putVar(start,count,&data[0]);
        } catch (netCDF::exceptions::NcException& e) {
            if (verbose) std::cout << e.what() << std::endl;
            resp = createErrorMsg(1, "Failed to add Geophysical Model to loop project file",verbose);
        }
    }
    return resp;
}

LoopProjectFileResponse GeophysicalModels::GetGeophysicalModel(netCDF::NcGroup* rootNode, std::vector<float>& data, std::vector<int>& dataShape, unsigned int index, bool verbose)
{
    LoopProjectFileResponse resp = {0,""};
    try {
        auto groups = rootNode->getGroups();
        if (groups.find("GeophysicalModels") == groups.end()) {
            resp = createErrorMsg(1,"No Geophysical Models Group in project file",verbose);
        } else {
            netCDF::NcGroup gmGroup = rootNode->getGroup("GeophysicalModels");
            netCDF::NcDim easting = gmGroup.getDim("easting");
            netCDF::NcDim northing = gmGroup.getDim("northing");
            netCDF::NcDim depth = gmGroup.getDim("depth");
            dataShape.clear();
            dataShape.push_back(easting.getSize());
            dataShape.push_back(northing.getSize());
            dataShape.push_back(depth.getSize());
            data.clear();
            data.resize(dataShape[0]*dataShape[1]*dataShape[2]);
            std::vector<size_t> start;
            std::vector<size_t> count;
            netCDF::NcVar dataVar = gmGroup.getVar("data");
            start.push_back(0); start.push_back(0); start.push_back(0); start.push_back(index);
            count.push_back(dataShape[0]); count.push_back(dataShape[1]); count.push_back(dataShape[2]); count.push_back(1);
            dataVar.getVar(start,count,&data[0]);
        }
    } catch (netCDF::exceptions::NcException& e) {
        if (verbose) std::cout << e.what() << std::endl;
        resp = createErrorMsg(1, "Failed to add Geophysical Model to loop project file",verbose);
    }
    return resp;
}

} // namespace LoopProjectFile