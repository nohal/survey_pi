# ~~~
# Summary:      Local, non-generic plugin setup
# Copyright (c) 2020-2021 Mike Rossiter
# License:      GPLv3+
# ~~~

# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation; either version 3 of the License, or (at your option) any later
# version.

# -------- Options ----------

set(OCPN_TEST_REPO
    "opencpn/survey-alpha"
    CACHE STRING "Default repository for untagged builds"
)
set(OCPN_BETA_REPO
    "opencpn/survey-beta"
    CACHE STRING "Default repository for tagged builds matching 'beta'"
)
set(OCPN_RELEASE_REPO
    "opencpn/survey-prod"
    CACHE STRING "Default repository for tagged builds not matching 'beta'"
)

option(SURVEY_USE_SVG "Use SVG graphics" ON)

#
# -------  Plugin setup --------
#
set(PKG_NAME survey_pi)
set(PKG_VERSION 4.0.0)
set(PKG_PRERELEASE "") # Empty, or a tag like 'beta'

set(DISPLAY_NAME survey) # Dialogs, installer artifacts, ...
set(PLUGIN_API_NAME survey) # As of GetCommonName() in plugin API
set(PKG_SUMMARY "Survey")
set(PKG_DESCRIPTION
    [=[
Survey
]=]
)

set(PKG_AUTHOR "Mike Rossiter")
set(PKG_IS_OPEN_SOURCE "yes")
set(PKG_HOMEPAGE https://github.com/Rasbats/survey_pi)
set(PKG_INFO_URL https://opencpn.org/OpenCPN/plugins/survey.html)

set(SRC
    src/baro_history.cpp
    src/baro_history.h
    src/bbox.cpp
    src/bbox.h
    src/circle.xpm
    src/cross.xpm
    src/dychart.h
    src/icons.cpp
    src/icons.h
    src/instrument.cpp
    src/instrument.h
    src/mygridtablebase.cpp
    src/mygridtablebase.h
    src/ProfileWin.cpp
    src/ProfileWin.h
    src/soundinggridtable.cpp
    src/soundinggridtable.h
    src/square.xpm
    src/surveygui.cpp
    src/surveygui.h
    src/surveygui_impl.cpp
    src/surveygui_impl.h
    src/SurveyOverlayFactory.cpp
    src/SurveyOverlayFactory.h
    src/survey_pi.cpp
    src/survey_pi.h
)

set(PKG_API_LIB api-17) # A directory in libs/ e. g., api-17 or api-16

macro (late_init)
  # Perform initialization after the PACKAGE_NAME library, compilers and
  # ocpn::api is available.
  add_definitions(-D SQLITE_ENABLE_RTREE)

  if (SURVEY_USE_SVG)
    target_compile_definitions(${PACKAGE_NAME} PUBLIC SURVEY_USE_SVG)
  endif ()
endmacro ()

macro (add_plugin_libraries)
  # Add libraries required by this plugin
  add_subdirectory("opencpn-libs/tinyxml")
  target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

  add_subdirectory("opencpn-libs/wxJSON")
  target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

  add_subdirectory("opencpn-libs/plugingl")
  target_link_libraries(${PACKAGE_NAME} ocpn::plugingl)

  add_subdirectory("opencpn-libs/jsoncpp")
  target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)

  add_subdirectory("opencpn-libs/nmea0183")
  target_link_libraries(${PACKAGE_NAME} ocpn::nmea0183)

  add_subdirectory("libs/sqlite3_i")
  target_link_libraries(${PACKAGE_NAME} sqlite3_i::sqlite3_i)

endmacro ()
