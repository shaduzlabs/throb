
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

cmake_minimum_required (VERSION 3.2.0)

project (throb)

set ( CMAKE_CXX_STANDARD 11 )

set( THROB_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR} )
set( THROB_BUILD_DIR ${CMAKE_BINARY_DIR} )
set( THROB_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR}/modules )
set( THROB_SOURCE_DIR ${THROB_ROOT_DIR}/src )

set( VST_INCLUDE_PATH "${THROB_MODULES_DIR}/vst3sdk" )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" )

include( cmake/JUCE.cmake )

if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  include( cmake/colors.cmake )
endif()

set_directory_properties( PROPERTIES
  COMPILE_DEFINITIONS         NDEBUG
  COMPILE_DEFINITIONS_DEBUG   DEBUG
  COMPILE_DEFINITIONS_RELEASE NDEBUG
)


# Version ---------------------------------------------------------------------------------------- #

string(TIMESTAMP THROB_BUILD_DATE "%Y-%m-%d")

set( THROB_VERSION_MAJOR 1 )
set( THROB_VERSION_MINOR 0 )
set( THROB_VERSION_MICRO 0 )
set( THROB_VERSION_HEX 0x010000 )

set( THROB_VERSION_STRING "${THROB_VERSION_MAJOR}.${THROB_VERSION_MINOR}.${THROB_VERSION_MICRO}")

if($ENV{APPVEYOR})
  set (THROB_BUILD_HOST "Appveyor")
  set (THROB_BUILD_NUMBER ".$ENV{APPVEYOR_BUILD_NUMBER}")
  set (THROB_GIT_BRANCH $ENV{APPVEYOR_REPO_BRANCH})
  execute_process(COMMAND appveyor UpdateBuild -Version ${THROB_VERSION_STRING})
elseif($ENV{TRAVIS})
  set (THROB_BUILD_HOST "Travis-CI")
  set (THROB_BUILD_NUMBER ".$ENV{TRAVIS_BUILD_NUMBER}")
  set (THROB_GIT_BRANCH $ENV{TRAVIS_BRANCH})
else()
  set (THROB_BUILD_HOST "a local machine")
  set (THROB_BUILD_NUMBER "")
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${THROB_ROOT_DIR}
    OUTPUT_VARIABLE THROB_GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif()

if(${THROB_GIT_BRANCH} STREQUAL "master")
  set(THROB_BUILD_TYPE "release version")
  set (THROB_VERSION_STRING "${THROB_VERSION_STRING}${THROB_BUILD_NUMBER}")
elseif(${THROB_GIT_BRANCH} STREQUAL "develop")
  set(THROB_BUILD_TYPE "development version")
  set (THROB_VERSION_STRING "${THROB_VERSION_STRING}${THROB_BUILD_NUMBER}-dev")
else()
  set(THROB_BUILD_TYPE "development version from feature branch: ${GIT_BRANCH}")
  set (THROB_VERSION_STRING "${THROB_VERSION_STRING}${THROB_BUILD_NUMBER}-fb")
endif()

configure_file (
  "${THROB_ROOT_DIR}/support/throb-config.h.in"
  "${PROJECT_BINARY_DIR}/throb-config.h"
)

if($ENV{APPVEYOR})
  execute_process(COMMAND appveyor UpdateBuild -Version ${THROB_VERSION_STRING})
  set(ENV{APPVEYOR_BUILD_VERSION} "${THROB_VERSION_STRING}")
elseif($ENV{TRAVIS})
  set(THROB_DEPLOY_TARGET $ENV{THROB_DEPLOY_TARGET})
  if(THROB_DEPLOY_TARGET)
    message(STATUS "Package ${THROB_DEPLOY_TARGET} will be deployed")
    configure_file (
      "${THROB_ROOT_DIR}/support/travis/deploy-bintray.json.in"
      "${PROJECT_BINARY_DIR}/deploy-bintray.json"
    )
  endif()

  set(COVERAGE_BUILD $ENV{COVERAGE_BUILD})
  if(COVERAGE_BUILD)
    set(COVERALLS ${COVERAGE_BUILD})
  endif()
endif()

set(
  THROB_BUILD_DESCRIPTION
  "cabl v. ${THROB_VERSION_STRING} - ${THROB_BUILD_TYPE} built on ${THROB_BUILD_HOST}"
)

MESSAGE( " ")
MESSAGE( "${White}              _______  __   __  ______    _______  _______  ")
MESSAGE( "${White}             |       ||  | |  ||    _ |  |       ||  _    | ")
MESSAGE( "${White}             |_     _||  |_|  ||   | ||  |   _   || |_|   | ")
MESSAGE( "${White}               |   |  |       ||   |_||_ |  | |  ||       | ")
MESSAGE( "${White}               |   |  |       ||    __  ||  |_|  ||  _   |  ")
MESSAGE( "${White}               |   |  |   _   ||   |  | ||       || |_|   | ")
MESSAGE( "${White}               |___|  |__| |__||___|  |_||_______||_______| ")
MESSAGE( "${BoldYellow}                                  Clock and reset generator${ColorReset}")

MESSAGE( "                                                  v. ${THROB_VERSION_STRING}")
MESSAGE( "                                               ")
message( STATUS "[${THROB_BUILD_DATE}] ${THROB_BUILD_DESCRIPTION} ")


# Configuration ---------------------------------------------------------------------------------- #

set( JUCE_CONFIG_PLUGIN_NAME throb )
set( JUCE_CONFIG_PLUGIN_DESC "Clock/Reset generator" )
set( JUCE_CONFIG_PLUGIN_MANUFACTURER shaduzLABS )
set( JUCE_CONFIG_PLUGIN_MANUFACTURER_URL www.shaduzlabs.com )
set( JUCE_CONFIG_PLUGIN_MANUFACTURER_EMAIL shaduz@shaduzlabs.com )
set( JUCE_CONFIG_PLUGIN_MANUFACTURER_CODE_STRING SLBS )
set( JUCE_CONFIG_PLUGIN_MANUFACTURER_CODE 0x534c4253 ) # 'SLBS'
set( JUCE_CONFIG_PLUGIN_CODE_STRING SLTB )
set( JUCE_CONFIG_PLUGIN_CODE 0x534c5442 ) # 'SLTB'
set( JUCE_CONFIG_IS_SYNTH 1 )
set( JUCE_CONFIG_WANTS_MIDI 0 )
set( JUCE_CONFIG_PRODUCES_MIDI 0 )
set( JUCE_CONFIG_IS_MIDI_EFFECT 0 )
set( JUCE_CONFIG_EDITOR_REQUIRES_KEYBOARD_FOCUS 0 )

set( JUCE_CONFIG_MAX_IN_CHANNELS 0 )
set( JUCE_CONFIG_MAX_OUT_CHANNELS 2 )

set( JUCE_CONFIG_VERSION ${THROB_VERSION_MAJOR}.${THROB_VERSION_MINOR}.${THROB_VERSION_MICRO} )
math(EXPR JUCE_CONFIG_VERSION_INTEGER "( ${THROB_VERSION_MAJOR} * 65536 ) + ( ${THROB_VERSION_MINOR} * 256 ) + ${THROB_VERSION_MICRO}" )
set( JUCE_CONFIG_VERSION_HEX ${THROB_VERSION_HEX} )
set( JUCE_CONFIG_VERSION_STRING ${THROB_VERSION_STRING} )

configure_file (
  "${THROB_ROOT_DIR}/support/JuceHeader.h.in"
  "${PROJECT_BINARY_DIR}/JuceHeader.h"
)

configure_file (
  "${THROB_ROOT_DIR}/support/AppConfig.h.in"
  "${PROJECT_BINARY_DIR}/clock-and-reset/AppConfig.h"
)

set( JUCE_CONFIG_MAX_OUT_CHANNELS 1 )
configure_file (
  "${THROB_ROOT_DIR}/support/AppConfig.h.in"
  "${PROJECT_BINARY_DIR}/clock-only/AppConfig.h"
)

set( THROB_CONFIG_CLOCK_AND_RESET ${PROJECT_BINARY_DIR}/clock-and-reset )
set( THROB_CONFIG_CLOCK_ONLY ${PROJECT_BINARY_DIR}/clock-only )


# Sources ---------------------------------------------------------------------------------------- #

SET ( THROB_SOURCES
  ${THROB_SOURCE_DIR}/ui/GraphicKnob.cpp
  ${THROB_SOURCE_DIR}/ui/GraphicKnob.h
  ${THROB_SOURCE_DIR}/ui/Led.cpp
  ${THROB_SOURCE_DIR}/ui/Led.h
  ${THROB_SOURCE_DIR}/ThrobGUI.cpp
  ${THROB_SOURCE_DIR}/ThrobGUI.h
  ${THROB_SOURCE_DIR}/PluginProcessor.cpp
  ${THROB_SOURCE_DIR}/PluginProcessor.h
  ${THROB_SOURCE_DIR}/PluginEditor.cpp
  ${THROB_SOURCE_DIR}/PluginEditor.h
)

include_directories( ${PROJECT_BINARY_DIR} ${THROB_ROOT_DIR}/support/JuceLibraryCode )


# Targets ---------------------------------------------------------------------------------------- #

if( APPLE )
  add_library (throbAU MODULE ${THROB_SOURCES} )
  target_include_directories(throbAU PRIVATE ${THROB_SOURCE_DIR} ${THROB_CONFIG_CLOCK_AND_RESET} )
  addJUCE_AU(throb throbAU)
endif( APPLE )

add_library (throbVST MODULE ${THROB_SOURCES} )
target_include_directories(throbVST PRIVATE ${THROB_SOURCE_DIR} ${THROB_CONFIG_CLOCK_AND_RESET} )
addJUCE_VST(throb throbVST)

add_library (throbVST3 MODULE ${THROB_SOURCES} )
target_include_directories(throbVST3 PRIVATE ${THROB_SOURCE_DIR} ${THROB_CONFIG_CLOCK_AND_RESET} )

if( WIN32 )
  addJUCE_VST3(throb-vst3 throbVST3)
else()
  addJUCE_VST3(throb throbVST3)
endif( WIN32 )
