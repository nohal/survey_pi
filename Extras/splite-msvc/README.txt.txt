The present binary distribution is exclusively
intended to support the MSCV C/C++ compiler 
[Win32], and includes the following packages: 

- GNU ICONV 
  [from the OSGeo4W binaries]

- PROJ.4 v.4.7.0 
  [built by Andrea Peri: MSVC 2008]

- GEOS v.3.3.1 
  [built by Sandro Furieri: MSVC 2010]

- FreeXL v.1.0.0 
  [built by Sandro Furieri: MSVC 2010]

- SQLite3 v.3.7.8 
  [built by Sandro Furieri: MSVC 2010]

- libspatialite 3.0.0-beta1
  [built by Sandro Furieri: MSVC 2010]

Please Note: the same libraries are available from
the OSGeo4W distribution.
But in many cases OSGeo4W still supports incredibly
old and obsolete versions: the present distribution
on the other side exclusively supports the most recent
version available for each package (with the excepction
of ICONV).

==============================================

Build Notes:

GEOS was built using CMake:
http://trac.osgeo.org/geos/wiki/BuildingOnWindowsWithCMake


FreeXL, SQLite3 and libspatialite were built
using the scripts you can find included into 
the "build-scripts" folder.

As a general rule, these build-scripts are supposed
to work both on MSVC 2008 and 2010.

General Setup:
--------------
the main folder is intented to be:
C:\splite-msvc

thus, the following layout is expected:

-+ C:
 +-+ splite-msvc
   +-+ bin [.exe and .dll files]
   +-+ include [header files]
   +-+ lib [.lib libraries]
 
in order to compile and link:
- open the Visual Studio Command Prompt
- goto the folder containing the sources

nmake /f makefile.vc
namke /f makefile.vc install



=====================================

How to check if the package is correctly installed:

- open a command shell:

> SET PATH=C:\splite-msvc\bin
> ECHO %PATH%
C:\splite-msvc\bin

> sqlite3
SQLite version 3.7.8 2011-09-19 14:49:19
Enter ".help" for instructions
Enter SQL statements terminated with a ";"
sqlite> SELECT load_extension("spatialite.dll");

sqlite> SELECT sqlite_version();
3.7.8
sqlite> SELECT spatialite_version();
3.0.0-beta1
sqlite> SELECT proj4_version();
Rel. 4.7.1, 23 September 2009
sqlite> SELECT geos_version();
3.3.1-CAPI-1.7.1
sqlite> .quit
>
