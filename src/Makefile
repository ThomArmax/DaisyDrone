# Project Name
TARGET = daisy_drone

# Sources
CPP_SOURCES = main.cpp

C_SOURCES = 
	
# Library Locations
LIBDAISY_DIR = ../libDaisy
DAISYSP_DIR = ../DaisySP

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# FATFS dir contains fatfs itself, libdaisy sys folder contains ffconf 
C_INCLUDES += -I$(LIBDAISY_DIR)/src/sys
