sudo apt-get install libpcre3-dev bison flex scons subversion


* apply patch to sources
* run ./configure inside swig sources  (or swigconfig.h not found)

v8 will fail:

in third_party/v8/v8 edit SConstruct, change:

   and remove -Werror lines


