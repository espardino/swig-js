/* -----------------------------------------------------------------------------
 * jsv8run.swg
 *
 * This file contains the runtime support for V8  modules
 * and includes code for managing global variables and pointer
 * type checking.
 * ----------------------------------------------------------------------------- */
 
/* For backward compatibility only */
#define SWIG_POINTER_EXCEPTION  0

/* for raw pointers */
#define SWIG_ConvertPtr(obj, pptr, type, flags)         	SWIG_JS_ConvertPtrAndOwn(obj, pptr, type, flags, 0)
#define SWIG_ConvertPtrAndOwn(obj, pptr, type, flags, own)  SWIG_JS_ConvertPtrAndOwn(obj, pptr, type, flags, own)
#define SWIG_NewPointerObj(ptr, type, flags)            	SWIG_JS_NewPointerObj(ptr, type, flags)
#define SWIG_AcquirePtr(ptr, own)                       	SWIG_JS_AcquirePtr(ptr, own)

/* for raw packed data */
#define SWIG_ConvertPacked(obj, ptr, sz, ty)
#define SWIG_NewPackedObj(ptr, sz, type)

/* for class or struct pointers */
#define SWIG_ConvertInstance(obj, pptr, type, flags)    SWIG_ConvertPtr(obj, pptr, type, flags)
#define SWIG_NewInstanceObj(ptr, type, flags) 			SWIG_NewPointerObj(ptr, type, flags)

/* for C or C++ function pointers */
#define SWIG_ConvertFunctionPtr(obj, pptr, type)        SWIG_ConvertPtr(obj, pptr, type, 0)
#define SWIG_NewFunctionPtrObj(ptr, type)               SWIG_NewPointerObj(ptr, type, 0)

/* for C++ member pointers, ie, member methods */
#define SWIG_ConvertMember(obj, ptr, sz, ty)
#define SWIG_NewMemberObj(ptr, sz, type)


/* -----------------------------------------------------------------------------
 * end jsv8run.swg
 * ----------------------------------------------------------------------------- */

 /* Runtime API */

#define SWIG_GetModule(clientdata)                  SWIG_JS_GetModule(clientdata)
#define SWIG_SetModule(clientdata, pointer) 		SWIG_JS_SetModule(clientdata, pointer)


/* Error manipulation */

#define SWIG_ErrorType(code)                            SWIG_JS_ErrorType(code)               
#define SWIG_Error(code, msg)            				SWIG_JS_Error(code,msg)
#define SWIG_fail                        				goto fail

void SWIG_JS_Error(int code, const char* msg) {
	v8::ThrowException(v8::String::New(msg));
}

swig_module_info* SWIG_JS_GetModule(void* clientdata) {
	return 0;
}

void
SWIG_JS_SetModule(void* clientdata, swig_module_info* module_info) {
	return;
}

v8::Handle<v8::Value>
SWIG_NewPointerObj(void* data, swig_type_info* typeInfo, int flags) {
    v8::Persistent<v8::FunctionTemplate> class_templ = *(reinterpret_cast<v8::Persistent<v8::FunctionTemplate>*>(typeInfo->clientdata));
    v8::Local<v8::Function> ctor = class_templ->GetFunction();
    v8::Local<v8::Object> obj = ctor->New();
    obj->SetInternalField(0, v8::External::New(data));
    
    return obj;
}
