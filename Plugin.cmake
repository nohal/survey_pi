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
	)


set(PKG_API_LIB api-17)  #  A directory in libs/ e. g., api-17 or api-16

macro(late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers
  # and ocpn::api is available.
  ADD_DEFINITIONS(-D SQLITE_ENABLE_RTREE)

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

  add_subdirectory("libs/sqlite3_i")
  target_link_libraries(${PACKAGE_NAME} sqlite3_i::sqlite3_i)
  
endmacro ()
