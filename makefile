# Basic Linux makefile for Loop Project File if not using CMake
TESTPROG=testingLoopProjectFile
PROJECT=LoopProjectFileCpp
LOOPPROJECTFILECPPLIB=libLoopProjectFileCpp.so

# Change the install directory as per your requirements
INSTALLDIR=~/sandpit

HDRS = LoopProjectFile.h \
		LoopVersion.h \
		LoopProjectFileUtils.h \
		LoopExtents.h \
		LoopDataCollection.h \
		LoopExtractedInformation.h \
		LoopStructuralModels.h \
		LoopGeophysicalModels.h \
		LoopUncertaintyModels.h
LIBSRCS = LoopProjectFile.cpp \
		LoopVersion.cpp \
		LoopProjectFileUtils.cpp \
		LoopExtents.cpp \
		LoopDataCollection.cpp \
		LoopExtractedInformation.cpp \
		LoopStructuralModels.cpp \
		LoopGeophysicalModels.cpp \
		LoopUncertaintyModels.cpp
LIBOBJS = $(LIBSRCS:.cpp=.o)
SRCS = $(TESTPROG).cpp
CXXFLAGS =

INCLUDES= 
LIBS= -L. -l$(PROJECT) -lnetcdf_c++4 -lnetcdf

$(LOOPPROJECTFILECPPLIB): $(HDRS) $(LIBSRCS)
	g++ $(CXXFLAGS) -c -fpic *.cpp
	g++ -shared $(CXXFLAGS) -o $(LOOPPROJECTFILECPPLIB) $(LIBOBJS)

$(TESTPROG): $(SRCS) $(HDRS) $(LOOPPROJECTFILECPPLIB)
	g++ $(CXXFLAGS) -o $(TESTPROG) $(SRCS) $(INCLUDES) $(LIBS)

all: clean $(LOOPPROJECTFILECPPLIB) $(TESTPROG) docs install

docs:
	doxygen Doxyfile
	cp loop_inv.png html/

install: docs
	install -m 755 -d $(INSTALLDIR)/lib
	install -m 640 -t $(INSTALLDIR)/lib $(LOOPPROJECTFILECPPLIB)
	install -m 755 -d $(INSTALLDIR)/include/LoopProjectFile-cpp
	install -m 640 -t $(INSTALLDIR)/include/LoopProjectFile-cpp $(HDRS)
	install -m 755 -d $(INSTALLDIR)/share/LoopProjectFile-cpp/html/search
	install -m 640 -t $(INSTALLDIR)/share/LoopProjectFile-cpp/html html/*
	install -m 640 -t $(INSTALLDIR)/share/LoopProjectFile-cpp/html/search html/search/*

clean:
	rm -f $(LOOPPROJECTFILECPPLIB)
	rm -f $(TESTPROG)
	rm -rf *.loop3d
	rm -f *.o
	rm -rf html/
	rm -rf latex/
