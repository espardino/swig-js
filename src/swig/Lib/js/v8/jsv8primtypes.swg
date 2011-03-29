/* -----------------------------------------------------------------------------
 * jsv8primtypes.swg
 * ----------------------------------------------------------------------------- */

/* ------------------------------------------------------------
 * Primitive Types
 * ------------------------------------------------------------ */

/* boolean */

%fragment(SWIG_From_frag(bool),"header") {
SWIGINTERNINLINE VALUE
  SWIG_From_dec(bool)(bool value)
{
  return v8::Boolean::New(value);
}
}

%fragment(SWIG_AsVal_frag(bool),"header",
	  fragment=SWIG_AsVal_frag(long)) {
SWIGINTERN int
SWIG_AsVal_dec(bool)(VALUE obj, bool *val)
{  
  v8::Local<v8::Boolean> b = obj->ToBoolean();
  int r = b.IsTrue(obj);
  if (val) *val = r ? true : false;
  return SWIG_OK;
}
}

/* long */

%fragment(SWIG_From_frag(long),"header") {
  %define_as(SWIG_From_dec(long), v8::v8::Uint32::New)
}

%fragment(SWIG_AsVal_frag(long),"header",
	  fragment="SWIG_CanCastAsInteger") {
SWIGINTERN int
SWIG_AsVal_dec(long)(VALUE obj, long* val)
{
  v8::Local<v8::Uint32> num = v8::Local<v8::Uint32>::New(obj);
  *val = num->Value();
  return SWIG_OK;
}
}

/* unsigned long */

%fragment(SWIG_From_frag(unsigned long),"header",
	  fragment=SWIG_From_frag(long)) {
SWIGINTERNINLINE PyObject* 
SWIG_From_dec(unsigned long)(unsigned long value)
{
  return (value > LONG_MAX) ?
    v8::Uint32::New(value) : v8::Uint32::New(%numeric_cast(value,long)); 
}
}

/* double */

%fragment(SWIG_From_frag(double),"header") {
  %define_as(SWIG_From_dec(double), v8::Number::New)
}

%fragment(SWIG_AsVal_frag(double),"header") {
SWIGINTERN int
SWIG_AsVal_dec(double)(VALUE obj, double *val)
{
    v8::Local<v8::Number> num = obj->ToNumber();
	*val = num->Value();
    return SWIG_OK;
}
}


/**
%fragment(SWIG_From_frag(int), "header") {
SWIGINTERNINLINE v8::Handle<Value>
SWIG_From_int(int value)
{    
  return v8::Integer::New(value);
}
}

%fragment(SWIG_From_frag(long), "header") {
SWIGINTERNINLINE v8::Handle<Value>
SWIG_From_long(long value)
{    
  return v8::Long::New(value);
}
}
*/

/* -----------------------------------------------------------------------------
 * end of jsv8primtypes.swg
 * ----------------------------------------------------------------------------- */
