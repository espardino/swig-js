/* ------------------------------------------------------------
 * jsv8.swg
 *
 * JavaScript V8 configuration module.
 * ------------------------------------------------------------ */

/* ------------------------------------------------------------
 *  Inner macros 
 * ------------------------------------------------------------ */
%include <jsv8macros.swg>

/* ------------------------------------------------------------
 *  The runtime part
 * ------------------------------------------------------------ */
%include <jsv8runtime.swg>

/* ------------------------------------------------------------
 *  Special user directives
 * ------------------------------------------------------------ */
%include <jsv8userdir.swg>

/* ------------------------------------------------------------
 *  Typemap specializations
 * ------------------------------------------------------------ */
%include <jsv8typemaps.swg>

/* ------------------------------------------------------------
 *  Overloaded operator support
 * ------------------------------------------------------------ */
%include <jsv8opers.swg>

/* ------------------------------------------------------------
 * Warnings for jsv8 keywords 
 * ------------------------------------------------------------ */
%include <jsv8kw.swg>

/* ------------------------------------------------------------
 * Documentation for common jsv8 methods 
 * ------------------------------------------------------------ */
//%include <jsv8autodoc.swg>

/* ------------------------------------------------------------
 * The jsv8 initialization function 
 * ------------------------------------------------------------ */
%include <jsv8init.swg>

/* Pointers, references */
%typemap(out,noblock=1) SWIGTYPE *, SWIGTYPE &, SWIGTYPE[] {
  %set_output(v8_set_new_instance(args, %as_voidptr($1)));
}

%typemap(out, noblock=1) SWIGTYPE *const& {
  %set_output(v8_set_new_instance(args, %as_voidptr(*$1)));
}

/* Return by value */
%typemap(out, noblock=1) SWIGTYPE {
  %set_output(v8_set_new_instance(args, %new_copy($1, $ltype)));
}
