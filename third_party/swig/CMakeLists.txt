
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/swig")
    message("Creating swig folder.")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/swig"
    )
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/swig/.svn")

    if( ${SVN_EXECUTABLE-NOTFOUND} )
        message(FATAL_ERROR "svn is necessary to checkout" )
    endif()

    set(SWIG_SVN_COMMAND 
        ${Subversion_SVN_EXECUTABLE} 
        co https://swig.svn.sourceforge.net/svnroot/swig/tags/rel-2.0.2
        ${CMAKE_CURRENT_SOURCE_DIR}/swig
    )
        
    message("Checking out swig 2.0.2: ${SWIG_SVN_COMMAND}")
    execute_process(
        COMMAND ${SWIG_SVN_COMMAND}
    )
    
endif()

if(WIN32)
    message("Creating swigconfig.h")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/swigconfig.h.win32 ${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/Include/swigconfig.h
    )
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
        PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/CParse"
                            "${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/Include"
                            "${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/DOH"
                            "${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/Swig"
                            "${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/Preprocessor"
                            "${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/Modules"
)                            

# include_directories( 
#                    "swig/Source/CParse"
#                    "swig/Source/Include"
#					"swig/Source/DOH" 
#					"swig/Source/Swig"
#					"swig/Source/Preprocessor"
#					"swig/Source/Modules")
					
# add the command to generate the source code
add_custom_command (
  OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/CParse/parser.c
  COMMAND bison -o ${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/CParse/parser.c --defines=${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/CParse/parser.h ${CMAKE_CURRENT_SOURCE_DIR}/swig/Source/CParse/parser.y
  )
					
add_library(swig_modules "swig/Source/Modules/allegrocl.cxx"
						"swig/Source/Modules/allocate.cxx"
						"swig/Source/Modules/browser.cxx"
						"swig/Source/Modules/cffi.cxx"
						"swig/Source/Modules/chicken.cxx"
						"swig/Source/Modules/clisp.cxx"
						"swig/Source/Modules/contract.cxx"
						"swig/Source/Modules/csharp.cxx"
						"swig/Source/Modules/directors.cxx"
						"swig/Source/Modules/emit.cxx"
						"swig/Source/Modules/guile.cxx"
						"swig/Source/Modules/java.cxx"
						"swig/Source/Modules/lang.cxx"
						"swig/Source/Modules/lua.cxx"
						"swig/Source/Modules/main.cxx"
						"swig/Source/Modules/modula3.cxx"
						"swig/Source/Modules/module.cxx"
						"swig/Source/Modules/mzscheme.cxx"
						"swig/Source/Modules/ocaml.cxx"
						"swig/Source/Modules/octave.cxx"
						"swig/Source/Modules/overload.cxx"
						"swig/Source/Modules/perl5.cxx"
						"swig/Source/Modules/php.cxx"
						"swig/Source/Modules/pike.cxx"
						"swig/Source/Modules/python.cxx"
						"swig/Source/Modules/r.cxx"
						"swig/Source/Modules/ruby.cxx"
						"swig/Source/Modules/s-exp.cxx"
						"swig/Source/Modules/tcl8.cxx"
						"swig/Source/Modules/typepass.cxx"
						"swig/Source/Modules/uffi.cxx"
						"swig/Source/Modules/utils.cxx"
						"swig/Source/Modules/xml.cxx"
)

add_library(swig_cparse "swig/Source/CParse/cscanner.c" 
						"swig/Source/CParse/templ.c" 
						"swig/Source/CParse/util.c"
						"swig/Source/CParse/parser.c"
)                        

add_library(swig_preprocessor "swig/Source/Preprocessor/cpp.c" 
						"swig/Source/Preprocessor/expr.c"
)

add_library(swig_doh	"swig/Source/DOH/base.c"
						"swig/Source/DOH/file.c"
						"swig/Source/DOH/fio.c"
						"swig/Source/DOH/hash.c"
						"swig/Source/DOH/list.c"
						"swig/Source/DOH/memory.c"
						"swig/Source/DOH/string.c"
						"swig/Source/DOH/void.c"
)
						
add_library(swiglib "swig/Source/Swig/cwrap.c"
				"swig/Source/Swig/deprecate.c"
				"swig/Source/Swig/error.c"
				"swig/Source/Swig/fragment.c"
				"swig/Source/Swig/getopt.c"
				"swig/Source/Swig/include.c"
				"swig/Source/Swig/misc.c"
				"swig/Source/Swig/naming.c"
				"swig/Source/Swig/parms.c"
				"swig/Source/Swig/scanner.c"
				"swig/Source/Swig/stype.c"
				"swig/Source/Swig/symbol.c"
				"swig/Source/Swig/tree.c"
				"swig/Source/Swig/typemap.c"
				"swig/Source/Swig/typeobj.c"
				"swig/Source/Swig/typesys.c"
				"swig/Source/Swig/wrapfunc.c"
)
