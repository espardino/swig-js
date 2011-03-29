 /* -----------------------------------------------------------------------------
 * jsv8api.swg
 * 
 * Defines some API methods for internal use in V8 extension.
 *
 * ----------------------------------------------------------------------------- */
#define JSV8_CLASS v8::Persistent<v8::FunctionTemplate>
 
v8::Persistent<v8::FunctionTemplate> v8_create_class_template(const char* symbol)
{
    v8::Local<v8::FunctionTemplate> class_templ = v8::FunctionTemplate::New();
    class_templ->SetClassName(v8::String::NewSymbol(symbol));
    
    v8::Handle<v8::ObjectTemplate> inst_templ = class_templ->InstanceTemplate();
    inst_templ->SetInternalFieldCount(1);
    
    return v8::Persistent<v8::FunctionTemplate>::New(class_templ);    
}

void v8_set_allocate_handler(v8::Persistent<v8::FunctionTemplate> class_templ, v8::InvocationCallback allocHandler)
{
    class_templ->SetCallHandler(allocHandler);
}

v8::Handle<v8::Value> v8_set_new_instance(const v8::Arguments& args, void* data) {
    v8::HandleScope scope;

    v8::Handle<v8::Object> self = args.Holder();
    self->SetInternalField(0, v8::External::New(data));

    return self;        
}

void v8_add_class_method(v8::Handle<v8::FunctionTemplate> class_templ, const char* symbol, v8::InvocationCallback _func)
{
    v8::Handle<v8::ObjectTemplate> proto_templ = class_templ->PrototypeTemplate();
    proto_templ->Set(v8::String::NewSymbol(symbol), v8::FunctionTemplate::New(_func));    
}

void v8_add_class_member_getters_setters(v8::Handle<v8::FunctionTemplate> class_templ, const char* varname, v8::AccessorGetter getter, v8::AccessorSetter setter) {
    v8::Handle<v8::ObjectTemplate> proto_templ = class_templ->InstanceTemplate();
    proto_templ->SetAccessor(v8::String::New(varname), getter, setter);
}

template <class T>
T* v8_unwrap_this_pointer (v8::Handle<v8::Object> handle) {
    assert(!handle.IsEmpty());
    assert(handle->InternalFieldCount() > 0);
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(handle->GetInternalField(0));
    return static_cast<T*>(wrap->Value());
}

/* -----------------------------------------------------------------------------
 * end of jsv8api.swg
 * ----------------------------------------------------------------------------- */
