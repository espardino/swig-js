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



#
# Prepare options for v8 scons build
# ==================================

# if(V8_SNAPSHOT)
# set(v8_snapshot snapshot=on)
# endif()

# if(V8_OPROFILE)
# set(v8_oprofile prof=on)
# endif()

# if(V8_GDBJIT)
    # if(NOT WIN32)
        # set(v8_gdbjit gdbjit=on)
    # endif()
# endif()

#if(OFF)
#set(v8_parallel_jobs "-j ${parallel_jobs}")
#endif()

if(NOT ${CMAKE_BUILD_TYPE} EQUAL "")
    string(TOLOWER ${CMAKE_BUILD_TYPE} v8mode)
else()
    set(v8mode "release")
endif()

if(${v8mode} STREQUAL "debug")
    if(WIN32)
        set(_V8_LIB ${CMAKE_CURRENT_SOURCE_DIR}/v8/v8_g.lib)
    else()
        message(FATAL_ERROR "Not yet implemented: what is the output of v8 build under posix?")
    endif()
elseif(${v8mode} STREQUAL "release")
    if(WIN32)
        set(_V8_LIB ${CMAKE_CURRENT_SOURCE_DIR}/v8/v8.lib)
    else()
        message(FATAL_ERROR "Not yet implemented: what is the output of v8 build under posix?")
    endif()
else()
    message(FATAL_ERROR "Illegal build mode for v8: ${v8mode}")
endif()


if(MSVC90)
    # HACK: default settings does not work for vc9 
    set(SCONS_ARGS ${SCONS_ARGS} "env=\"PATH:%PATH%,INCLUDE:%INCLUDE%,LIB:%LIB%\"")
endif()
    
add_custom_command(
    OUTPUT BUILD_V8.rule
    COMMAND ${Scons_EXECUTABLE} ${SCONS_ARGS} mode=${v8mode} verbose=on
    COMMAND ${CMAKE_COMMAND} -E touch BUILD_V8.rule
    COMMENT "Building v8 using scons:  ${Scons_EXECUTABLE} ${SCONS_ARGS} mode=${v8mode} verbose=on"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/v8
)

add_custom_target(v8 DEPENDS BUILD_V8.rule)

set_target_properties(v8
                      PROPERTIES FOLDER third_party
)