Swig-js
=======

This project addresses two separate swig modules that can be used to create Javascript bindings for existing C(++) libraries.

 - FireBreath Module
        A swig module that generates FireBreath code that can be integrated into FireBreah projects.
        FireBreath produces a NPAPI/ActiveX plugins that are cross-browser compatible.
        This is currently the best way to extend a browser with custom native libraries.
        
 - V8 Module
        A swig module that generates V8 glueing code. V8 is a high-performance javascript enging that is used by browser Chrome.
        V8 is a quite lightweight API that can be integrated quite easily into a standalone application.
        Though, currently there is no easy way to extend an existing browser by means of V8 extensions.
        And due to security reasons this won't happen in future. Nevertheless, there are efforts to bind V8 to the Qt-Webkit-Browser widget.
        It could be possible to extend such an integrated browser having a lightweight application that can host modern Web-Apps and simultanously 
        can be extended with v8 extensions.


Requirements
------------

 - git
 - flex/bison (in PATH)
 - svn (in PATH)


Building Swig-js
----------------
 
 - Create a clone
 
 - Initialize git submodules

    > git submodule init
    > git submodule update
    
 - Create build directory, e.g., 
    > mkdir build/win32_vc9
 
 - Change to the build directory
    > cd build/win32_vc9
    
 - Run CMake
    > cmake -G "Visual Studio 9 2008" ../..
    Note: specify the relative path to root directory of swig-js clone
