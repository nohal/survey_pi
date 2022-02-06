# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.


# -------- Options ----------

set(OCPN_TEST_REPO
    "opencpn/survey-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "opencpn/survey-beta"
    CACHE STRING
    "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "opencpn/survey-prod"
    CACHE STRING
    "Default repository for tagged builds not matching 'beta'"
)

option(SURVEY_USE_SVG "Use SVG graphics" ON)

#
#
# -------  Plugin setup --------
#
set(PKG_NAME survey_pi)
set(PKG_VERSION  4.0.0)
set(PKG_PRERELEASE "")  # Empty, or a tag like 'beta'

set(DISPLAY_NAME survey)    # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME survey) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "Survey")
set(PKG_DESCRIPTION [=[
Survey
]=])

set(PKG_AUTHOR "Mike Rossiter")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/Rasbats/survey_pi)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/survey.html)

IF(WIN32)
	SET(sqlite ${CMAKE_SOURCE_DIR}/sqlite-msvc/lib/sqlite3_i.lib)
	SET(sqlite "${PARENT}.lib")
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/sqlite-msvc/include)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/sqlite-msvc/lib)
ENDIF(WIN32)

SET(SRC
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


set(PKG_API_LIB api-16)  #  A directory in libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
  ADD_DEFINITIONS(-D SQLITE_ENABLE_RTREE)

  IF(WIN32)
	FIND_LIBRARY(sqlite3_i ${CMAKE_SOURCE_DIR}/sqlite-msvc/lib)
	TARGET_LINK_LIBRARIES(${PACKAGE_NAME} sqlite3_i)
  ENDIF(WIN32)
  
  if (SURVEY_USE_SVG)
    target_compile_definitions(${PACKAGE_NAME} PUBLIC SURVEY_USE_SVG)
  endif ()
endmacro ()

macro(add_plugin_libraries)
  # Add libraries required by this plugin
  add_subdirectory("libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

  add_subdirectory("libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

  add_subdirectory("libs/plugingl")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)

  add_subdirectory("libs/jsoncpp")
  target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)

  add_subdirectory("libs/nmea0183")
  target_link_libraries(${PACKAGE_NAME} nmea0183::nmea0183)
  
endmacro ()