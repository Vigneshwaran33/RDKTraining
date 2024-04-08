# If not stated otherwise in this file or this component's LICENSE file the
# following copyright and licenses apply:
#
# Copyright 2020 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(PLUGIN_NAME GstPlayerCi)
set(MODULE_NAME WPEFramework${PLUGIN_NAME})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(PLUGIN_GstPlayerCi_AUTOSTART "true" CACHE STRING "Automatically start GstPlayerCi plugin")
set(PLUGIN_GstPlayerCi_MODE "Local" CACHE STRING "Controls if the plugin should run in its own process, in process or remote")
set(PLUGIN_GstPlayerCi_STARTUPORDER "" CACHE STRING "To configure startup order of GstPlayerCi plugin")

find_package(${NAMESPACE}Plugins REQUIRED)
find_package(DS)
find_package(IARMBus)
find_package(GLIB REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GST REQUIRED gstreamer-1.0)

find_library(GLIB_LIBRARY NAMES glib-2.0 )

include_directories(${GST_INCLUDE_DIRS})

add_library(${MODULE_NAME} SHARED
    Module.cpp
    GstPlayerCi.cpp
    GstPlayerCiImplementation.cpp
    lib.cpp
    )

set_target_properties(${MODULE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES)


target_include_directories(${MODULE_NAME}
        PRIVATE
        ../helpers
        ./
        ${IARMBUS_INCLUDE_DIRS}
        ${DS_INCLUDE_DIRS}
        ${GLIB_INCLUDE_DIRS}
        ${GST_INCLUDE_DIRS})

target_link_libraries(${MODULE_NAME}
        PRIVATE
        ${NAMESPACE}Plugins::${NAMESPACE}Plugins
        -lessos
        ${IARMBUS_LIBRARIES}
        ${DS_LIBRARIES}
        ${GLIB_LIBRARIES}
        ${GST_LIBRARIES})


add_definitions(-DDS_FOUND)
install(TARGETS ${MODULE_NAME} 
    DESTINATION lib/${STORAGE_DIRECTORY}/plugins)

write_config(${PLUGIN_NAME})
