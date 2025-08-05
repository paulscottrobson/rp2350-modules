# *******************************************************************************************
# *******************************************************************************************
#
#       Name :      system.make
#       Purpose :   Top level make include
#       Date :      5th July 2025
#       Author :    Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************

# Operating system detection
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    OS_NAME := linux
endif

ifeq ($(UNAME_S),Darwin)
    OS_NAME := macOS
endif

ifeq ($(OS),Windows_NT)
    OS_NAME := Windows
endif

#@echo "Operation System found: $(OS_NAME)"

ROOTDIR =  $(dir $(realpath $(lastword $(MAKEFILE_LIST))))../
include $(ROOTDIR)environment/common.$(OS_NAME).make

