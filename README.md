# FTOGL4
FTOGL4 is Opengl 4.0 port of FTOGL. Currently it supports outline, polygon and extrude fonts. 

# To compile and run 

## Directory Structure
```bash
FTOGL4
├── CcG3DVec
├── CcGPUCtrlLib
├── FTGL-OGL4
├── FTOGL4-MS19
├── FreeType
├── Images
├── glad
├── glfw
├── glm
└── readme.md
```

## Download
1. FTGL-OGL4 from github (Clone this repository or click on download zip)
2. Glad (Source and header files are included in the main project. No need for separate compile)
3. Glfw (glfw-3.3.4.bin.WIN64)
4. Glm (glm-0.9.9.8 - Header include files, So no need to compile)
5. Freetype

## Compile
1. CcG3DVec.lib
2. CcGPUCtrl.lib
3. FTGL-OGL4.lib

## Link (already linked in the project)
glfw3_mt.lib
opengl32.lib
GLU32.lib
freetype.lib
CcGPUCtrl.lib
CcG3DVec.lib
FTGL-OGL4.lib

## Run 
TestPrg.cpp - is a testing file under FTOGL4-MS19 folder 
