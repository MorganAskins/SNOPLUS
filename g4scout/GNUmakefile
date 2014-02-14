# $Id: GNUmakefile,v 1.17 2006/09/04 15:43:27 t2k Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

G4DEBUG = 0

name := Scout
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

ROOTCFLAGS   := $(shell root-config --cflags) -DUSE_ROOT -fPIC
ROOTLIBS     := $(shell root-config --libs)

#LIBNAME := RMSim_dev

# NOTE: Geant4.7.0 changes the way Maximum Step size is defined.  
# We need extra code for versions 4.7.0 and above; eventually 
# everyone should upgrade to geant4.7
ifneq (,$(findstring 4.7,$(G4INSTALL)))
GEANT4_7_0 = 0
else
GEANT4_7_0 = 1
endif

ifdef GEANT4_7_0
CPPFLAGS += -DGEANT4_7_0
endif

ifdef GCCVERS296
CPPFLAGS += -DUSE_STRSTREAM
endif

CPPFLAGS  += -I$(ROOTSYS)/include $(ROOTCFLAGS) 
EXTRALIBS += $(ROOTLIBS)

EXTRA_LINK_DEPENDENCIES := 

CPPFLAGS += -std=c++11

.PHONY: all
all: lib bin

# Note dependencies not yet set up right yet

include $(G4INSTALL)/config/binmake.gmk
