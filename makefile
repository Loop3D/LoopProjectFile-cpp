# Basic Linux makefile for Loop Project File if not using CMake
TESTPROG=testingLoopProjectFile

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
SRCS = $(TESTPROG).cpp

INCLUDES= 
LIBS= -lnetcdf_c++4 -lnetcdf

$(TESTPROG): $(SRCS) $(HDRS)
	g++ -o $(TESTPROG) $(SRCS) $(INCLUDES) $(LIBS)

all: clean $(TESTPROG) docs install

docs:
	doxygen Doxyfile
	cp loop_inv.png html/

install: docs
	install -m 755 -d $(INSTALLDIR)/include/LoopProjectFile-cpp
	install -m 640 -t $(INSTALLDIR)/include/LoopProjectFile-cpp $(HDRS)
	install -m 755 -d $(INSTALLDIR)/share/LoopProjectFile-cpp/html/search
	install -m 640 -t $(INSTALLDIR)/share/LoopProjectFile-cpp/html html/*
	install -m 640 -t $(INSTALLDIR)/share/LoopProjectFile-cpp/html/search html/search/*

clean:
	rm -f $(TESTPROG)
	rm -rf *.loop3d
	rm -f *.o
	rm -rf html/
	rm -rf latex/
