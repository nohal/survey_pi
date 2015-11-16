This build-script will create and install libspatialite:

- bin\spatialiite.dll
- lib\spatialite.lib [static library]
- lib\spatialite_i.lib [stub library to link against DLL]
- include\spatialite.h [header files]
- include\spatialite\*.*


============================

REMARK:
pay close attention to Some absolutely relevant parameters 
defined in "nmake.opt":

------
/D_LARGE_FILE=1 /D_FILE_OFFSET_BITS=64 /D_LARGEFILE_SOURCE=1

required so to correctly support DB-files exceding 2GB size.

------ 	
/DGEOS_ADVANCED=1

required so to enable GEOS v.3.3.x advanced features

-------
/DDLL_EXPORT

required in order to build both the DLL and the
static library