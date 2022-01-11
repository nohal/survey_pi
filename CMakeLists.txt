##---------------------------------------------------------------------------
## Author:      nohal aka. Pavel Kalian
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(survey_pi)

SET(PACKAGE_NAME survey_pi)
SET(VERBOSE_NAME survey_pi)
SET(TITLE_NAME survey_pi)
SET(CPACK_PACKAGE_CONTACT "Mike Rossiter")
#SET(CMAKE_BUILD_TYPE Debug)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)

SET(VERSION_MAJOR "3")
SET(VERSION_MINOR "523")

SET(NAME_SUFFIX "ov50")

set (CMAKE_CXX_STANDARD 11)

#SET(CMAKE_BUILD_TYPE Debug)

OPTION(SURVEY_USE_SVG "Use SVG graphics" ON)

INCLUDE("cmake/PluginConfigure.cmake")

IF(SURVEY_USE_SVG)
  ADD_DEFINITIONS(-DSURVEY_USE_SVG)
ENDIF(SURVEY_USE_SVG)


IF( NOT CMAKE_BUILD_TYPE )
  SET( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
       FORCE )
ENDIF( NOT CMAKE_BUILD_TYPE )


IF(WIN32)
	SET(sqlite ${CMAKE_SOURCE_DIR}/sqlite-msvc/lib/sqlite3_i.lib)
	SET(sqlite "${PARENT}.lib")
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/sqlite-msvc/include)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/sqlite-msvc/lib)
ENDIF(WIN32)

#  Search for wxWidgets 2.9 or newer
STRING(REGEX MATCHALL "-2.9|-3." wx_Version_2.9+ "${wxWidgets_INCLUDE_DIRS}")
IF(wx_Version_2.9+)
ELSE(wx_Version_2.9+)
#
#
ENDIF(wx_Version_2.9+)

SET( wxWidgets_USE_DEBUG OFF)
SET( wxWidgets_USE_UNICODE ON)
SET( wxWidgets_USE_UNIVERSAL OFF)
SET( wxWidgets_USE_STATIC OFF)

INCLUDE("cmake/PluginConfigure.cmake")

# For convenience we define the sources as a variable. You can add
# header files and cpp/c files and CMake will sort them out

SET(SRC_NMEA0183
		src/nmea0183/LatLong.hpp
		src/nmea0183/latlong.cpp
		src/nmea0183/long.cpp
		src/nmea0183/nmea0183.cpp
		src/nmea0183/nmea0183.hpp
		src/nmea0183/Response.hpp
		src/nmea0183/response.cpp
		src/nmea0183/RMB.hpp
		src/nmea0183/rmb.cpp
		src/nmea0183/Sentence.hpp
		src/nmea0183/sentence.cpp
		src/nmea0183/talkerid.cpp
		src/nmea0183/RMC.HPP
		src/nmea0183/rmc.cpp
		src/nmea0183/hexvalue.cpp
		src/nmea0183/lat.cpp
		src/nmea0183/expid.cpp
		src/nmea0183/wpl.hpp
		src/nmea0183/wpl.cpp
		src/nmea0183/rte.hpp
		src/nmea0183/rte.cpp
		src/nmea0183/hdt.hpp
		src/nmea0183/hdt.cpp
		src/nmea0183/hdg.hpp
		src/nmea0183/hdg.cpp
		src/nmea0183/hdm.hpp
		src/nmea0183/hdm.cpp
		src/nmea0183/gll.hpp
		src/nmea0183/gll.cpp
		src/nmea0183/vtg.hpp
		src/nmea0183/vtg.cpp
		src/nmea0183/gga.hpp
		src/nmea0183/gga.cpp
		src/nmea0183/gsv.hpp
		src/nmea0183/gsv.cpp
        src/nmea0183/dbt.cpp
        src/nmea0183/dbt.hpp
        src/nmea0183/dpt.cpp
        src/nmea0183/dpt.hpp
        src/nmea0183/mtw.cpp
        src/nmea0183/mtw.hpp
        src/nmea0183/mwv.cpp
        src/nmea0183/mwv.hpp
        src/nmea0183/vhw.hpp
        src/nmea0183/vhw.cpp
        src/nmea0183/vwr.cpp
        src/nmea0183/vwr.hpp
        src/nmea0183/zda.cpp
        src/nmea0183/zda.hpp
		src/nmea0183/rsa.hpp
		src/nmea0183/rsa.cpp
		src/nmea0183/SatInfo.h
        src/nmea0183/mwd.cpp
        src/nmea0183/mwd.hpp
        src/nmea0183/vwt.cpp
        src/nmea0183/vwt.hpp
	)
INCLUDE_DIRECTORIES(src/nmea0183)

SET(SRC_SURVEY
    src/survey_pi.h
    src/survey_pi.cpp
    src/icons.h
    src/icons.cpp
	src/surveygui.h
	src/surveygui.cpp
	src/surveygui_impl.cpp
	src/surveygui_impl.h
	src/mygridtablebase.cpp
	src/mygridtablebase.h
	src/soundinggridtable.cpp
	src/soundinggridtable.h
	src/ProfileWin.cpp
	src/ProfileWin.h
	src/baro_history.cpp
	src/baro_history.h
	src/instrument.cpp
	src/instrument.h
	src/circle.xpm
	src/cross.xpm
	src/square.xpm
    src/bbox.cpp
	src/bbox.h
	src/SurveyOverlayFactory.cpp
	src/SurveyOverlayFactory.h
	src/dychart.h
	src/ocpn_plugin.h
	)

SET(SRC_TINYXML
	src/tinyxml.h
	src/tinyxml.cpp
    src/tinyxmlparser.cpp
    src/tinyxmlerror.cpp
	)

IF(TINYXML_FOUND)
    message (STATUS "Building with tinyxml includes")
    INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
    ADD_DEFINITIONS(-DTIXML_USE_STL)
ELSE(TINYXML_FOUND)
    message (STATUS "tinyxml library not found")
    SET(SRC_SURVEY ${SRC_SURVEY} ${SRC_TINYXML})
    ADD_DEFINITIONS(-DTIXML_USE_STL)
ENDIF(TINYXML_FOUND)

ADD_DEFINITIONS(-D SQLITE_ENABLE_RTREE)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_SURVEY} ${SRC_NMEA0183})

IF(WIN32)
	FIND_LIBRARY(sqlite3_i ${CMAKE_SOURCE_DIR}/sqlite-msvc/lib)
	TARGET_LINK_LIBRARIES(${PACKAGE_NAME} sqlite3_i)
ENDIF(WIN32)

ADD_DEFINITIONS(-DTIXML_USE_STL)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
