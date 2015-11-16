This build-script will create and install libsqlite3:

- bin\sqlite3.dll
- lib\sqlite3.lib [static library]
- lib\sqlite3_i.lib [stub library to link against DLL]
- include\sqlite3.h [header files]
- include\sqlite3ext.h

-----------------------------------------------------

The command line front end "sqlite3.exe" will be
created and installed as well:

- bin\sqlite3.exe

Please Note: this executable dependes on sqlite3.dll !!!


============================

REMARK:
pay close attention to Some absolutely relevant parameters 
defined in "nmake.opt":

------
/D_LARGE_FILE=1 /D_FILE_OFFSET_BITS=64 /D_LARGEFILE_SOURCE=1

required so to correctly support DB-files exceding 2GB size.

------ 	
/DSQLITE_ENABLE_RTREE=1 

required so to enable the R*Tree Spatial Index support

-------
/DSQLITE_ENABLE_LOAD_EXTENSION=1

required so to enable dynamic extensions loading

-------

/DSQLITE_API=__declspec(dllexport) 
/DDLL_EXPORT

required in order to build both the DLL and the
static library