include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
               OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/example_wrap.cxx
               COMMAND ${SWIG_JS_EXECUTABLE} -jsv8 -c++ -debug-top 4 -o ${CMAKE_CURRENT_BINARY_DIR}/example_wrap.cxx -outdir ${CMAKE_CURRENT_BINARY_DIR} example.i > ${CMAKE_CURRENT_BINARY_DIR}/output.txt
               COMMENT "${SWIG_JS_EXECUTABLE} -jsv8 -c++ -debug-top 4 -o ${CMAKE_CURRENT_BINARY_DIR}/example_wrap.cxx -outdir ${CMAKE_CURRENT_BINARY_DIR} example.i > ${CMAKE_CURRENT_BINARY_DIR}/output.txt"
               WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(jsv8_class
               example.cxx
               example.h
               main.cxx
               ${CMAKE_CURRENT_BINARY_DIR}/example_wrap.cxx
)

add_dependencies(jsv8_class v8 swig)
target_link_libraries(jsv8_class ${V8_LINK_LIBS})
set_target_properties(jsv8_class PROPERTIES FOLDER "examples/v8")

if (NOT EXISTS ${PROJECT_BINARY_DIR}/bin)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/bin"
    )
endif()

get_target_property(class_exe jsv8_class LOCATION)
add_custom_command(
    TARGET jsv8_class
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${class_exe} ${PROJECT_BINARY_DIR}/bin
)
