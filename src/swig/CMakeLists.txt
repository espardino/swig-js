
#
# V8 Swig Module:
#

get_directory_property(SWIG_INCLUDES DIRECTORY ${SWIG_SOURCE_DIR} INCLUDE_DIRECTORIES)

include_directories(${SWIG_INCLUDES})

add_library(swig_v8 v8/jsv8.cxx)

#
# swig.exe
#

add_executable(swig_exe swigmain.cxx)

target_link_libraries(swig_exe 
                      swig_cparse swig_preprocessor swig_doh swiglib swig_modules 
                      swig_v8)
