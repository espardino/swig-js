/* -----------------------------------------------------------------------------
 * jsv8errors.swg
 * ----------------------------------------------------------------------------- */ 
 
// TODO: map Swig errors to v8 errors
SWIGINTERN VALUE
SWIG_Js_ErrorType(int SWIG_code) {
  VALUE type;
  switch (SWIG_code) {
  case SWIG_MemoryError:
  case SWIG_IOError:
  case SWIG_RuntimeError:
  case SWIG_IndexError:
  case SWIG_TypeError:
  case SWIG_DivisionByZero:
  case SWIG_OverflowError:
  case SWIG_SyntaxError:
  case SWIG_ValueError:
  case SWIG_SystemError:
  case SWIG_AttributeError:
  case SWIG_NullReferenceError:
  case SWIG_UnknownError:
  default:
    type = v8::Undefined();
  }
  return type;
}

/* -----------------------------------------------------------------------------
 * end of jsv8errors.swg
 * ----------------------------------------------------------------------------- */ 
