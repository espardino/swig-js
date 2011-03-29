include_directories(${V8_DIR}/include)

if(WIN32)
    list(APPEND V8_LINK_LIBS "WINMM.LIB" ${V8_LIBS})
else()
    list(APPEND V8_LINK_LIBS ${V8_LIBS})
endif()

add_subdirectory(class)
