# ~~~
# Summary:      sqlite3_i headers library cmake setup
# License:      GPLv3+
# Copyright (c) 2022 Mike Rossiter
#
# Include sqlite3_i header and, on win32, link import lib. 
# ~~~

cmake_minimum_required(VERSION 3.1)

if (TARGET ocpn::sqlite3_i)
  return ()
endif ()


add_library(SQLITE3_I INTERFACE)
add_library(sqlite3_i::sqlite3_i ALIAS SQLITE3_I)
target_include_directories(SQLITE3_I INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
if (WIN32 AND MSVC)
  target_link_libraries(
  SQLITE3_I INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/sqlite3_i.lib")
  target_compile_definitions(SQLITE3_I INTERFACE -DUNICODE)
endif ()

