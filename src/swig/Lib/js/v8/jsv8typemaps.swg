/* -----------------------------------------------------------------------------
 * jsv8typemaps.swg
 * ----------------------------------------------------------------------------- */

/* ------------------------------------------------------------
 *  Typemap specializations for jsv8
 * ------------------------------------------------------------ */

 /* ------------------------------------------------------------
 *  Fragment section
 * ------------------------------------------------------------ */

/* Include fundamental fragemt definitions */
%include <typemaps/fragments.swg>

/* Look for user fragments file. */
//%include <jsv8fragments.swg>

/* jsv8 fragments for primitive types */
%include <jsv8primtypes.swg>

/* jsv8 fragments for char* strings */
//%include <jsv8strings.swg>

/* ------------------------------------------------------------
 *  Unified typemap section
 * ------------------------------------------------------------ */

/**
 * Note: the following definitions are mandatory; used by swigtypemaps.swig.
 */
#define SWIG_Object                      v8::Handle<Value>
#define VOID_Object                      v8::Undefined()
#define SWIG_AppendOutput(result,obj)    SWIG_Js_AppendOutput(result, obj)
#define SWIG_SetConstant(name, obj)      SWIG_Js_SetConstant(name, obj)
#define SWIG_Raise(obj, type, desc)      SWIG_Js_Raise(obj, type, desc)

/* Include the unified typemap library */
%include <typemaps/swigtypemaps.swg>

/* -----------------------------------------------------------------------------
 * end of jsv8typemaps.swg
 * ----------------------------------------------------------------------------- */
