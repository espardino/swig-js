#
# FireBreath
# ==========
#

if(WIN32)
	add_subdirectory(FireBreath)

	set_target_properties(NpapiCore PluginCore ScriptingCore ActiveXCore 
		              PROPERTIES FOLDER third_party/FireBreath
	)
	set_target_properties(UnitTest++ UnitTest_ActiveXCore  UnitTest_ScriptingCore NPAPIHost
		              PROPERTIES FOLDER third_party/FireBreath/UnitTests
	)
	set_target_properties(boost_thread boost_system 
		              PROPERTIES FOLDER third_party/boost
	)
endif()
#
# Swig
# ==========
#

add_subdirectory(swig)
set_target_properties(swig_cparse swig_preprocessor swig_doh swiglib swig_modules 
                      PROPERTIES FOLDER third_party/swig
)

#
# v8
# ==
#

add_subdirectory(v8)
