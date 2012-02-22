#
# Checkout v8
# ===========

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/v8/.svn")

    if( ${SVN_EXECUTABLE-NOTFOUND} )
        message(FATAL_ERROR "svn is necessary to checkout" )
    endif()

    set(V8_SVN_COMMAND 
        ${Subversion_SVN_EXECUTABLE} 
        co http://v8.googlecode.com/svn/tags/3.2.4
        ${CMAKE_CURRENT_SOURCE_DIR}/v8
    )
        
    message("Checking out v8 3.2.4: ${V8_SVN_COMMAND}")
    execute_process(
        COMMAND ${V8_SVN_COMMAND}
    )
    
endif()


# Prepare options for v8 scons build
# ==================================

if(NOT ${CMAKE_BUILD_TYPE} EQUAL "")
    string(TOLOWER ${CMAKE_BUILD_TYPE} v8mode)
else()
    set(v8mode "release")
endif()

if(${v8mode} STREQUAL "debug")
    if(WIN32)
        list(APPEND _V8_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/v8/v8.lib)
        set(V8_DLL ${CMAKE_CURRENT_SOURCE_DIR}/v8/v8_g.dll)
    elseif(UNIX)
        list(APPEND _V8_LIBS -lv8)
        set(V8_DLL ${CMAKE_CURRENT_SOURCE_DIR}/v8/libv8.so)
    else()
        message(FATAL_ERROR "Not yet implemented: what is the output of v8 build under posix?")
    endif()
elseif(${v8mode} STREQUAL "release")
    if(WIN32)
        list(APPEND _V8_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/v8/v8.lib)
        set(V8_DLL ${CMAKE_CURRENT_SOURCE_DIR}/v8/v8.dll)
    elseif(UNIX)
        list(APPEND _V8_LIBS -lv8)
        set(V8_DLL ${CMAKE_CURRENT_SOURCE_DIR}/v8/libv8.so)
    else()
        message(FATAL_ERROR "Not yet implemented: what is the output of v8 build under posix?")
    endif()
else()
    message(FATAL_ERROR "Illegal build mode for v8: ${v8mode}")
endif()

list(APPEND SCONS_ARGS mode=${v8mode} library=shared)

if(MSVC90)
    list(APPEND SCONS_ARGS env="PATH:%PATH%,INCLUDE:%INCLUDE%,LIB:%LIB%")
endif()

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/BUILD_V8)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/BUILD_V8 "Blaaa")
endif()

add_custom_command(
    OUTPUT ${V8_DLL}
    COMMAND ${Scons_EXECUTABLE} ${SCONS_ARGS} verbose=on
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/v8/ChangeLog
    #COMMENT "Building v8 using scons:  ${Scons_EXECUTABLE} ${SCONS_ARGS} mode=${v8mode} verbose=on"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/v8
)

add_custom_target(v8 ALL DEPENDS ${V8_DLL})

set_target_properties(v8
        PROPERTIES FOLDER third_party
)

add_custom_command(TARGET v8 POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${V8_DLL} ${PROJECT_BINARY_DIR}/bin/)

set (V8_LIBS ${_V8_LIBS} CACHE INTERNAL "" FORCE)
set (V8_DIR ${CMAKE_CURRENT_SOURCE_DIR}/v8 CACHE INTERNAL "" FORCE)
