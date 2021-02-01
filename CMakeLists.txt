cmake_minimum_required(VERSION 3.10)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(LPF_LIBRARY_NAME LoopProjectFileCppD)
else()
    set(LPF_LIBRARY_NAME LoopProjectFileCpp)
endif()
set(LPF_TEST_PROG TestingLoopProjectFile)
set(CMAKE_CXX_STANDARD 11)

project(${LPF_TEST_PROG})

set(HEADERS LoopProjectFile.h
            LoopProjectFileUtils.h
            LoopExtents.h
            LoopDataCollection.h
            LoopExtractedInformation.h
            LoopGeophysicalModels.h
            LoopStructuralModels.h
            LoopUncertaintyModels.h
            LoopVersion.h
            )
set(SRCS LoopProjectFile.cpp
            LoopProjectFileUtils.cpp
            LoopExtents.cpp
            LoopDataCollection.cpp
            LoopExtractedInformation.cpp
            LoopGeophysicalModels.cpp
            LoopStructuralModels.cpp
            LoopUncertaintyModels.cpp
            LoopVersion.cpp
            )
add_library(${LPF_LIBRARY_NAME} ${SRCS})
add_executable(${LPF_TEST_PROG} testingLoopProjectFile.cpp)
target_link_libraries(${LPF_TEST_PROG} ${LPF_LIBRARY_NAME})


# Find netCDF dependancy
find_package(NetCDF REQUIRED)
if (NetCDF_FOUND)
    message("NetCDF Found")
    include_directories(${NETCDF_INCLUDES})
    target_link_libraries(${LPF_TEST_PROG} ${NETCDF_LIBRARIES})
    target_link_libraries(${LPF_TEST_PROG} netcdf)
    if (UNIX)
        target_link_libraries(${LPF_TEST_PROG} netcdf netcdf_c++4)
    endif(UNIX)
else ()
    message("NetCDF NOT Found")
endif ()

# Create documentation with Doxygen in Linux environment
if (UNIX)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set( DOXYGEN_OUTPUT_DIRECTORY doxygen )
        set( DOXYGEN_COLLABORATION_GRAPH YES )
        set( DOXYGEN_EXTRACT_ALL NO )
        set( DOXYGEN_CLASS_DIAGRAMS YES )
        set( DOXYGEN_HIDE_UNDOC_RELATIONS YES )
        set( DOXYGEN_HAVE_DOT YES )
        set( DOXYGEN_CLASS_GRAPH YES )
        set( DOXYGEN_CALL_GRAPH YES )
        set( DOXYGEN_CALLER_GRAPH NO )
        set( DOXYGEN_COLLABORATION_GRAPH YES )
        set( DOXYGEN_BUILTIN_STL_SUPPORT YES )
        set( DOXYGEN_EXTRACT_PRIVATE NO )
        set( DOXYGEN_EXTRACT_PACKAGE NO )
        set( DOXYGEN_EXTRACT_STATIC NO )
        set( DOXYGEN_EXTRACT_LOCALMETHODS NO )
        set( DOXYGEN_UML_LOOK NO )
        set( DOXYGEN_UML_LIMIT_NUM_FIELDS 50 )
        set( DOXYGEN_TEMPLATE_RELATIONS NO )
        set( DOXYGEN_DOT_GRAPH_MAX_NODES 50 )
        set( DOXYGEN_MAX_DOT_GRAPH_DEPTH 0 )
        set( DOXYGEN_DOT_TRANSPARENT NO )
        set( DOXTGEN_USE_MDFILE_AS_MAINPAGE README.md )
        set( DOXYGEN_FULL_PATH_NAMES YES )
        set( DOXYGEN_STRIP_FROM_PATH ${PROJECT_SOURCE_DIR} )
        doxygen_add_docs( docs ${PROJECT_SOURCE_DIR} )
    else()
        message("Doxygen needs to be installed to generate Doxygen docs")
    endif()
endif()

# Setup installation

if (WIN32)
    install(TARGETS ${LPF_TEST_PROG} DESTINATION "LoopProjectFile-cpp/")
    install(TARGETS ${LPF_LIBRARY_NAME} DESTINATION "LoopProjectFile-cpp/")
    install(FILES ${HEADERS} DESTINATION "LoopProjectFile-cpp/")
endif()
if (UNIX)
    install(TARGETS ${LPF_TEST_PROG} DESTINATION "bin")
    install(TARGETS ${LPF_LIBRARY_NAME} DESTINATION "lib")
    install(FILES ${HEADERS} DESTINATION "include/LoopProjectFile-cpp/")
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen/html DESTINATION "share/LoopProjectFile-cpp/")
    install(FILES loop_inv.png DESTINATION "share/LoopProjectFile-cpp/html/")
endif()
