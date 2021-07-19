# FTOGL4
FTOGL4 is Opengl 4.0 port of FTGL. Currently, it supports outline, polygon and extrude fonts.
And uses reduced version of OpenGL related Libraries provided by ConsCAD for Opensource. (ConsCAD is a non-opensource software solution for Architectures).

# Pre-requisites
1. MS Windows 10
2. MS studio 2019 community version x64 (or any other)
3. Hardware and driver to support OpenGL4.
4. External dependancies. FTOGL4 uses opensource Libraries: glfw, glm, freetype and glad
5. Window fonts available at location "C:\WINDOWS\Fonts\arial.ttf". 
(if not, then change fontFilePathL = "C:\\WINDOWS\\Fonts\\arial.ttf" in file 'TestPrg.cpp' line No. 114 appropriately)

# To compile and run 
## Directory Structure
Following should be the directory structure after cloning the repository. glfw, glm, freetype and glad are to be created as folders in the root directory. Requires download and copy into this folders appropriately.
(If any of these library already availbale in your machine and want to use it then please change the path setting accordingly in the project)
 
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


## Download (dependencies) 
1. glfw
(We have used pre-compiled binaries for 64-bit Windows "glfw-3.3.4.bin.WIN64")
Download 64-bit Windows pre-compiled binaries from  https://www.glfw.org/download.html 
This will download  'glfw-3.3.4.bin.WIN64.zip'. Unzip and copy it a way to have
FTOGL4\glfw\lib-vc2019
(FYI We use : glfw\lib-vc2019\glfw3_mt.lib)

(Optional Compile: User can download 'Source package' and compile glfw as per procedure mentioned on their site.)

2. glm 
GLM (OpenGL Mathematics, a header-only library)
https://github.com/g-truc/glm/tags
Under Tag 0.9.9.8 - click on zip. This will download 0.9.9.8.zip.
Unzip the same and copy it in a way to have
\FTOGL4\glm
(glm will have folders like cmake, doc, glm, test, util and some files)

3. Freetype - 
User can download from https://www.freetype.org/download.html
User may Download Windows DLLs of FreeType  directly from a github repository (version 2.10.4, built with VS Express 2017).
https://github.com/ubawurinna/freetype-windows-binaries
Unzip and copy in a way to have 
FTOGL4\FreeType\release static\win64
(FYI We use : FreeType\release static\win64\freetype.lib )


4. glad
(GLAD uses a web service to load all relevant OpenGL functions according to the version of OpenGL (We use OpenGL-4.0).
User can use the GLAD web service page https://glad.dav1d.de/
Please make sure to set 
language : C++, 
Specification : OpenGL
API: gl : 4.0
(gles1, gles2, glsc2 are None)
profile : Core
(Ignore the extensions )

Option:
Tick -> Generate a loader
Click 'Generate' Button  to produce the zip file. 
This leads to a page 'https://glad.dav1d.de/generated/tmp9zchz23hglad/' (This may change)
Click on glad.zip to download it.
unzip glad.zip and copy it in a way to have 
E:\FTOGL4\glad
(\glad  will have two folders include (Has two folders Include { has two folders : glad [glad.h] and KHR [khrplatform.h]} and src [glad.c])



## Build - Solution 

Open the solution
FTOGL4-MS19.sln from folder .\FTOGL4\FTOGL4-MS19
Soultion Explorer should show follwoing 
Solution 'FTOGL4-MS19' (4 OF 4 projects)
1. CcG3DVec (Reduced version of Library provided by ConsCAD for Opensource)
2. CcGPUCtrl (Reduced version of Library provided by ConsCAD for Opensource)
3. FTGL-OGL4 (Modified FTGL for OpenGL4)
4. FTOGL4-MS19 (Test application to test FTOGL4)

## Build (Compile and link)
1. CcG3DVec to generate CcG3DVec.lib;
2. CcGPUCtrl to generate CcGPUCtrl.lib;
3. FTGL-OGL4 to generate FTGL-OGL4.lib;
4. FTOGL4-MS19  to generate Application
(Above Libraries and application, generated in folder : \FTOGL4\FTOGL4-MS19\x64\Debug)

Build solution (F7) should Compile and link without any error. 
Then Run the application (F5 or Ctrl+F5) should open the window displaying all the types of Fonts.
Good Luck :


Please Note:
FTOGL4-MS19.sln has the nesessary path set for the compile and link, 
(if erased or you have differnt path then follow procedure in "Set Path")



### Set Path (optional) 
Project -> Property -> c/c++ -> General : Additional include Directories

1. CcG3DVec
Additional include Directories:
(Do not have any Includes)

2. CcGPUCtrl
Additional include Directories:
.\..\..\glm; .\..\..\CcG3DVec; .\..\..\glad\include; .\..\..\FreeType\include;

3. FTGL-OGL4
Additional include Directories:
.\..\glm;.\..\CcG3DVec;.\..\glad\include;.\..\FreeType\include;.\..\CcGPUCtrlLib\CcGPUCtrl;./include;./include/FTGL;./include/FTGlyph;./include/FTFont-Impl;./include/FTGlyph-Impl;./include/FTLayout;


4. FTOGL4-MS19
Additional include Directories
.\..\glad\include;.\..\glfw\include;.\..\CcGPUCtrlLib\CcGPUCtrl;.\..\glm;.\..\CcG3DVec;.\..\FreeType\include;.\..\FTGL-OGL4\Include;%(AdditionalIncludeDirectories)

Application also to include Library setting in
Project -> Property -> Linker -> General : Additional Library Directories
.\..\glfw\lib-vc2019;.\..\FTOGL4-MS19\x64\Debug;.\..\FreeType\release static\win64;.\..\FTOGL4-MS19\Debug;

Project -> Property -> Linker -> Input : Additional Dependancy 
glfw3_mt.lib;opengl32.lib;CcGPUCtrl.lib;CcG3DVec.lib;freetype.lib;FTGL-OGL4.lib;GLU32.lib;

