
        ##########    Copyright (C) 2017 Vincenzo Pacella
        ##      ##    Distributed under MIT license, see file LICENSE
        ##      ##    or <http://opensource.org/licenses/MIT>
        ##      ##
##########      ############################################################# shaduzlabs.com #######

set(SUBMODULES_DIR ${CMAKE_SOURCE_DIR}/modules)
set(JUCE_LIBRARY_CODE_DIR ${CMAKE_SOURCE_DIR}/support/JuceLibraryCode/)

# ----------------------------------------------------------------------------------------------------------------------

function( addJUCE target )

  if(APPLE)
    find_package( OpenGL REQUIRED )
  endif()

  target_compile_definitions( ${target} PRIVATE
    JUCE_SHARED_CODE=1
    JucePlugin_Build_VST=0
    JucePlugin_Build_VST3=0
    JucePlugin_Build_AU=0
    JucePlugin_Build_AUv3=0
    JucePlugin_Build_RTAS=0
    JucePlugin_Build_AAX=0
    JucePlugin_Build_Standalone=0
  )

  set(juce_library
    ${JUCE_LIBRARY_CODE_DIR}/juce_core.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_cryptography.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_data_structures.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_events.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_graphics.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_gui_basics.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_gui_extra.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_opengl.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_video.cpp
  )

  set(juce_library_audio
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_basics.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_devices.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_formats.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_processors.cpp
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_utils.cpp
  )

  set(juce_library_audio_plugin_client
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_utils.cpp
  )

  set(juce_library_audio_plugin_client_aax
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_AAX.cpp
  )

  set(juce_library_audio_plugin_client_aax_osx
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_AAX.mm
  )

  set(juce_library_audio_plugin_client_standalone
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_Standalone.cpp
  )

  target_sources( ${target} PRIVATE ${juce_library} )
  target_sources( ${target} PRIVATE ${juce_library_audio} )
  target_sources( ${target} PRIVATE ${juce_library_audio_plugin_client} )

  target_include_directories( ${target} PUBLIC ${JUCE_LIBRARY_CODE_DIR} ${CMAKE_SOURCE_DIR}/modules/JUCE/modules )

  set_target_properties(${target} PROPERTIES
    COMPILE_DEFINITIONS         NDEBUG
    COMPILE_DEFINITIONS_DEBUG   DEBUG
    COMPILE_DEFINITIONS_RELEASE NDEBUG
  )

  if( APPLE )

    set_target_properties( ${target} PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -x objective-c++" )
    target_link_libraries( ${target} PRIVATE ${OPENGL_gl_LIBRARY} )
    target_link_libraries( ${target} PUBLIC
      "-framework AudioUnit"
      "-framework AudioToolbox"
      "-framework Carbon"
      "-framework Cocoa"
      "-framework CoreFoundation"
      "-framework CoreAudio"
      "-framework CoreMidi"
      "-framework QuartzCore"
      "-framework IOKit"
      "-framework AGL"
      "-framework Accelerate"
      "-framework WebKit"
      "-lobjc"
    )

  elseif(WIN32)

    target_link_libraries(${target} PUBLIC
      advapi32.lib
      comdlg32.lib
      gdi32.lib
      GlU32.Lib
      kernel32.lib
      ole32.lib
      OpenGL32.Lib
      rpcrt4.lib
      shell32.lib
      user32.lib
      vfw32.lib
      wininet.lib
      winmm.lib
      ws2_32.lib
    )

  endif()

endfunction(addJUCE)

# ----------------------------------------------------------------------------------------------------------------------

function( addJUCE_VST name target )
  addJUCE(${target})

  target_compile_definitions( ${target} PRIVATE JucePlugin_Build_VST=1 )

  set(juce_library_audio_plugin_client_vst
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_VST2.cpp
  )

  target_include_directories( ${target} PUBLIC ${VST_INCLUDE_PATH} )
  target_sources(${target} PRIVATE ${juce_library_audio_plugin_client_vst})

  if(APPLE)
    configure_file (
      "${THROB_ROOT_DIR}/support/osx/Info-VSTx.plist.in"
      "${THROB_BUILD_DIR}/plists/Info-VST.plist"
    )
    set(juce_library_audio_plugin_client_vst_osx
      ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_VST_utils.mm
    )
    target_sources(${target} PRIVATE ${juce_library_audio_plugin_client_vst_osx})
  endif()

  set_target_properties(${target} PROPERTIES
    BUNDLE true
    OUTPUT_NAME ${name}
    BUNDLE_EXTENSION "vst"
    XCODE_ATTRIBUTE_WRAPPER_EXTENSION "vst"
    MACOSX_BUNDLE_INFO_PLIST "${THROB_BUILD_DIR}/plists/Info-VST.plist"
  )

  if(APPLE)
    install( TARGETS ${target} DESTINATION "$ENV{HOME}/Library/Audio/Plug-Ins/VST" )
  endif()

endfunction( addJUCE_VST )

# ----------------------------------------------------------------------------------------------------------------------

function( addJUCE_VST3 name target )
  addJUCE(${target})

  target_compile_definitions( ${target} PRIVATE JucePlugin_Build_VST3=1 )

  set(juce_library_audio_plugin_client_vst
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_VST2.cpp
  )

  set(juce_library_audio_plugin_client_vst3
    ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_VST3.cpp
  )

  target_include_directories( ${target} PUBLIC ${VST_INCLUDE_PATH} )
  target_sources(${target} PRIVATE ${juce_library_audio_plugin_client_vst})
  target_sources(${target} PRIVATE ${juce_library_audio_plugin_client_vst3})
  if(APPLE)
    configure_file (
      "${THROB_ROOT_DIR}/support/osx/Info-VSTx.plist.in"
      "${THROB_BUILD_DIR}/plists/Info-VST3.plist"
    )
    set(juce_library_audio_plugin_client_vst_osx
      ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_VST_utils.mm
    )
    target_sources(${target} PRIVATE ${juce_library_audio_plugin_client_vst_osx})
  endif()

  set_target_properties(${target} PROPERTIES
    BUNDLE true
    OUTPUT_NAME ${name}
    BUNDLE_EXTENSION "vst3"
    XCODE_ATTRIBUTE_WRAPPER_EXTENSION "vst3"
    MACOSX_BUNDLE_INFO_PLIST "${THROB_BUILD_DIR}/plists/Info-VST3.plist"
  )

  if(APPLE)
    install( TARGETS ${target} DESTINATION "$ENV{HOME}/Library/Audio/Plug-Ins/VST3" )
  endif()

endfunction( addJUCE_VST3 )

# ----------------------------------------------------------------------------------------------------------------------

function( addJUCE_AU name target )
  if(APPLE)
    addJUCE(${target})

    configure_file (
      "${THROB_ROOT_DIR}/support/osx/Info-AU.plist.in"
      "${THROB_BUILD_DIR}/plists/Info-AU.plist"
    )

    target_compile_definitions( ${target} PRIVATE JucePlugin_Build_AU=1 )

    set(juce_library_audio_plugin_client_au
      ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_AU_1.mm
      ${JUCE_LIBRARY_CODE_DIR}/juce_audio_plugin_client_AU_2.mm
    )

    target_sources(${target} PRIVATE ${juce_library_audio_plugin_client_au})

    set_target_properties( ${target} PROPERTIES
      BUNDLE true
      OUTPUT_NAME ${name}
      BUNDLE_EXTENSION "component"
      XCODE_ATTRIBUTE_WRAPPER_EXTENSION "component"
      MACOSX_BUNDLE_INFO_PLIST "${THROB_BUILD_DIR}/plists/Info-AU.plist"
    )

    install( TARGETS ${target} DESTINATION "$ENV{HOME}/Library/Audio/Plug-Ins/Components" )
  endif()
endfunction(addJUCE_AU)

# ----------------------------------------------------------------------------------------------------------------------
