
#
# V8 Swig Module:
#

get_directory_property(SWIG_INCLUDES DIRECTORY ${SWIG_SOURCE_DIR} INCLUDE_DIRECTORIES)

include_directories(${SWIG_INCLUDES})

add_executable(swig swigmain.cxx jsv8.cxx)

if (UNIX)
target_link_libraries(swig
                      swig_cparse swig_preprocessor swig_doh swiglib swig_modules libpcre.so)
else()
target_link_libraries(swig
                      swig_cparse swig_preprocessor swig_doh swiglib swig_modules )
endif()
get_target_property(SWIG_EXE swig LOCATION)
get_filename_component(SWIG_EXE_NAME ${SWIG_EXE} NAME)

add_custom_command(TARGET swig
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy ${SWIG_EXE} ${PROJECT_BINARY_DIR}
)

add_custom_command(TARGET swig
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${SWIG_SOURCE_DIR}/swig/Lib ${PROJECT_BINARY_DIR}/Lib
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Lib ${PROJECT_BINARY_DIR}/Lib
)

set(SWIG_JS_EXECUTABLE ${PROJECT_BINARY_DIR}/${SWIG_EXE_NAME} CACHE INTERNAL "" FORCE)
