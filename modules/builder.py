# *******************************************************************************************
# *******************************************************************************************
#
#      Name :      builder.py
#      Purpose :   Generates a CMakeList.txt for a collection of modules.
#      Date :      7th August 2025
#      Author :    Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,re,sys

# *******************************************************************************************
#
#                                   A single module
#
# *******************************************************************************************

class Module(object):
    def __init__(self,moduleName):
        assert os.path.isdir(moduleName),"Module directory {0} does not exist".format(moduleName)
        self.moduleName = moduleName
        self.includeFile = moduleName+"_module.h"
        self.dependencyPosition = 0
        self.dependencies = [x.strip() for x in open(moduleName+os.sep+"dependencies.info").readlines() if not x.startswith("#") and x.strip() != ""]

    def __str__(self):
        return "[{0}] inc={1} dep={2}:{3}".format(self.moduleName,self.includeFile,self.dependencyPosition,",".join(self.dependencies))

    def getModuleName(self):
        return self.moduleName
    def getIncludeFile(self):
        return self.includeFile
    def getDependencyPosition(self):
        return self.dependencyPosition
    def setDependencyPosition(self,n):
        self.dependencyPosition = n

# *******************************************************************************************
#
#                                 A collection of modules
#
# *******************************************************************************************

class ModuleSet(object):
    def __init__(self):
        self.modules = {}
        self.addModule("common")
    #
    #       Add a module if it does not exist.
    #
    def addModule(self,module):
        while module.endswith(os.sep):
            module = module[:-1]
        if module not in self.modules:
            self.modules[module] = Module(module)
            for m in self.modules[module].dependencies:
                self.addModule(m)
    #
    #       Assign levels to the modules so every modules dependencies have a lesser dependency position.
    #       Then create sort the modules in the sortedModules list with the lowest positions first.
    #
    def assignLevels(self):
        for m in self.modules.keys():
            self.modules[m].setDependencyPosition(0)
        sorted = False
        while not sorted:
            sorted = True
            for m in self.modules.keys():
                if not self.checkDependencies(self.modules[m]):
                    self.modules[m].setDependencyPosition(self.modules[m].getDependencyPosition()+1)
                    #print("Pushing {0} to {1}".format(m,self.modules[m].getDependencyPosition()))
                    sorted = False
        #
        self.sortedModules = [x for x in self.modules.keys()]
        self.sortedModules.sort(key = lambda x:self.modules[x].dependencyPosition)
    #
    #       For a given module, check all its dependencies are less than the module's own dependency.
    #
    def checkDependencies(self,module):
        for dep in module.dependencies:
            if self.modules[dep].dependencyPosition >= module.dependencyPosition:
                return False
        return True
    #
    #       Create a new file.
    #
    def createFile(self,fileName):
        assert not os.path.exists(fileName),"File {0} already exists".format(fileName)
        return open(fileName,"w")
    #
    #       Render the new build
    #
    def renderBuild(self,projectName):
        ms.assignLevels()
        self.createDirectories(projectName)
        self.renderCMakeLists(self.createFile(projectName+os.sep+"CMakeLists.txt"),projectName)
        self.renderMain(self.createFile(projectName+os.sep+"app"+os.sep+"main.c"),projectName)
        self.createInclude(projectName)
        self.createDocuments(projectName)
        self.createDependencyFile(projectName)
        self.createMakeFile(projectName)
    #
    #       Create project directories
    #
    def createDirectories(self,projectName):
        if not os.path.isdir(projectName):
            os.mkdir(projectName)
        for sd in ["documents","include","library","app"]:
            if not os.path.isdir(projectName+os.sep+sd):
                os.mkdir(projectName+os.sep+sd)
    #
    #       Create dummy documents
    #
    def createDocuments(self,projectName):
        self.createFile(projectName+os.sep+"documents"+os.sep+projectName+".md").write("# Documentation for "+projectName+"\n")
    #
    #       Create dummy include
    #
    def createInclude(self,projectName):
        includeFile = "||#pragma once|#include <stdlib.h>|#include <stdio.h>|#include <string.h>|#include <stdint.h>|#include <stdbool.h>||"
        h = self.createFile(projectName+os.sep+"include"+os.sep+projectName+"_module.h")
        h.write(includeFile.replace("|","\n"))
        for m in self.sortedModules:
            h.write("#include \"{0}_module.h\"\n".format(m))
        includeFile = "||#pragma once|#ifndef RUNTIME|#endif"
        self.createFile(projectName+os.sep+"include"+os.sep+projectName+"_module_local.h").write(includeFile.replace("|","\n"))
    #
    #       Create default makefile.
    #
    def createMakeFile(self,projectName):
        makeFile = "include ../../environment/system.make|APPNAME = {0}|include $(BUILDENVDIR)pico.$(OS).make".format(projectName)
        self.createFile(projectName+os.sep+"Makefile").write(makeFile.replace("|","\n"))
    #
    #       Create Dependencies
    #
    def createDependencyFile(self,projectName):
        self.createFile(projectName+os.sep+"dependencies.info").write("#\n#\tDependencies\n#\n"+"\n".join(self.sortedModules))
    #
    #       Render the makelist file.
    #
    def renderCMakeLists(self,h,projectName):
        header = "cmake_minimum_required(VERSION 3.12)||include(pico_sdk_import.cmake)||project(filetest)||pico_sdk_init()||option(USE_DEBUG \"Build with debug support\" ON) |if (USE_DEBUG)| add_definitions(-DDEBUG)|endif()||include_directories(include)"
        header = header.replace("filetest",projectName)
        h.write("\n".join(header.split("|")))

        h.write("\n")
        h.write("\n".join(["include_directories(${{MODULEDIR}}/{0}/include)".format(x) for x in self.sortedModules]))
        h.write("\n\n")

        if "dvi" in self.modules:
            h.write("add_subdirectory(${DVIDIR} lib)\n")

        h.write('file(GLOB_RECURSE APP_SOURCES "app/*.[csS]")\n')
        h.write("file(GLOB_RECURSE C_SOURCES \"library/*.[csS]\")\n")
        for m in self.sortedModules:
            h.write("file(GLOB_RECURSE {0}_MODULE_SOURCES \"${{MODULEDIR}}/{1}/library/*.[csS]\")\n".format(m.upper(),m))
        h.write("\nadd_executable({0}\n\t${{APP_SOURCES}} ${{C_SOURCES}}\n".format(projectName))
        h.write("\t"+" ".join(["${{{0}_MODULE_SOURCES}}".format(c.upper()) for c in self.sortedModules]))
        h.write("\n)\n")

        if "dvi" in self.modules:
            h.write("target_compile_definitions({0} PRIVATE\n".format(projectName));
            h.write("    DVI_USE_SIO_TMDS_ENCODER=0\n");
            h.write("    DVI_VERTICAL_REPEAT=1\n");
            h.write("    DVI_N_TMDS_BUFFERS=3\n");
            h.write("    DVI_1BPP_BIT_REVERSE=1\n");
            h.write(")\n\n");

        libs = "pico_stdlib,pico_multicore,pico_util,hardware_dma,pico_sync,libdvi"
        if "usb" in self.modules:
            libs += ",tinyusb_host,tinyusb_board"
        h.write("\ntarget_link_libraries({0} PUBLIC\n".format(projectName))        
        for l in libs.split(","):
            h.write("\t{0}\n".format(l))
        h.write(")\n")
        h.write("\npico_add_extra_outputs({0})\n".format(projectName))
    #
    #       Render the Main file.
    #
    def renderMain(self,h,projectName):
        h.write("#include \"{0}_module.h\"\n".format(projectName))
        #h.write("#include \"{0}_module_local.h\"\n".format(projectName))
        h.write("\n\nint MAINPROGRAM(int argc,char *argv[]) {\n\treturn 0;\n}\n")

if __name__ == "__main__":
    outputDirectory = None
    if len(sys.argv) == 1:
        print("builder.py <module> <modules> -o<target>")
        sys.exit(1)

    ms = ModuleSet()
    for s in sys.argv[1:]:
        if s.startswith("-o"):
            outputDirectory = s[2:]
        else:
            ms.addModule(s)

    if outputDirectory is None:
        print("No output directory specified.")
        sys.exit(1)
    else:
        ms.renderBuild(outputDirectory)
