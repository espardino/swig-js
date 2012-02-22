/* ----------------------------------------------------------------------------- 
* This file is part of SWIG, which is licensed as a whole under version 3 
* (or any later version) of the GNU General Public License. Some additional
* terms also apply to certain portions of SWIG. The full details of the SWIG
* license and copyrights can be found in the LICENSE and COPYRIGHT files
* included with the SWIG source code as distributed by the SWIG developers
* and at http://www.swig.org/legal.html.
*
* jsv8.cxx
*
* JavaScript (V8) language module for SWIG.
* ----------------------------------------------------------------------------- */

char cvsroot_jsv8_cxx[] = "$Id: jsv8.cxx xxxxx xxxx-xx-xx xx:xx:xxx xxx $";

#include "swigmod.h"
#include "cparse.h"
static int treduce = SWIG_cparse_template_reduce(0);

#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <iostream>



static const char *usage = "\
                           JavaScript Options (available with -js)\n\
                           ";
/*                           
int JSV8_DOH_EndsWith(String* s, String* suffix) {
    if (s == 0 || suffix == 0)
        return 0;
    
    int s_len = Len(s);
    int suffix_len = Len(suffix);
    
    if(s_len < suffix_len)
        return 0;
        
    char* s_str = Char(s);
    char* s_suffix = Char(suffix);
    
    s_str += s_len - suffix_len;
    return (strcmp(s_str, s_suffix) == 0);
}

*/


#define EndsWith(name,val) (Strcmp(Char(name) + Len(name) - 4, val)==0)
/**
 * Creates a string that is used to define a class template instance.
 */
void JSV8_GetClassTemplStr(Node* classNode, String* out) {
    String* clazzName = Getattr(classNode, "name");
    Printv(out, clazzName, "_class_templ", NIL);
}

class JavaScriptV8: public Language {
private:

    String *module;
    String *modvar;
    String *feature;
    String *prefix;
    int current;

    File *f_directors;
    File *f_directors_h;
    File *f_directors_helpers;
    File *f_begin;
    File *f_runtime;
    File *f_runtime_h;
    File *f_header;
    File *f_wrappers;
    File *f_init;
    File *f_initbeforefunc;

    // Wrap modes
    enum WrapperMode {
        NO_CPP,
        MEMBER_FUNC,
        CONSTRUCTOR,
        CONSTRUCTOR_ALLOCATE,
        CONSTRUCTOR_INITIALIZE,
        DESTRUCTOR,
        MEMBER_VAR,
        CLASS_CONST,
        STATIC_FUNC,
        STATIC_VAR
    };

public:

    /* ---------------------------------------------------------------------
    * JavaScriptV8()
    *
    * Initialize member data
    * --------------------------------------------------------------------- */

    JavaScriptV8() {
        module = 0;
        modvar = 0;
        feature = 0;
        prefix = 0;
        current = NO_CPP;
        f_begin = 0;
        f_runtime = 0;
        f_header = 0;
        f_wrappers = 0;
        f_init = 0;
        f_initbeforefunc = 0;
        director_multiple_inheritance = 0;
        director_language = 0;
    }
    
    /* ---------------------------------------------------------------------
    * validate_identifier()
    *
    * Is this a valid identifier in the scripting language?
    *
    * js method names can include any combination of letters, numbers
    * and underscores. 
    *
    * --------------------------------------------------------------------- */
    virtual int validate_identifier(String *s) {
        char *c = Char(s);
        while (*c) {
            if (!( isalnum(*c) || (*c == '_') ))
                return 0;
            c++;
        }
        return 1;
    }

    /* ---------------------------------------------------------------------
    * top()
    *
    * TODO(019): set a better name wrapper attribute, s. Java
    * --------------------------------------------------------------------- */
    virtual int top(Node *n) {

        /**
        * See if any module options have been specified as options
        * to the %module directive.
        */
        Node *swigModule = Getattr(n, "module");

        /* Initialize all of the output files */
        String *outfile = Getattr(n, "outfile");
        String *outfile_h = Getattr(n, "outfile_h");

        if (!outfile) {
            Printf(stderr, "Unable to determine outfile\n");
            SWIG_exit(EXIT_FAILURE);
        }

        f_begin = NewFile(outfile, "w", SWIG_output_files());
        if (!f_begin) {
            FileErrorDisplay(outfile);
            SWIG_exit(EXIT_FAILURE);
        }

        f_runtime = NewString("");
        f_init = NewString("");
        f_header = NewString("");
        f_wrappers = NewString("");
        f_directors_h = NewString("");
        f_directors = NewString("");
        f_directors_helpers = NewString("");
        f_initbeforefunc = NewString("");

        /* Register file targets with the SWIG file handler */
        Swig_register_filebyname("header", f_header);
        Swig_register_filebyname("wrapper", f_wrappers);
        Swig_register_filebyname("begin", f_begin);
        Swig_register_filebyname("runtime", f_runtime);
        Swig_register_filebyname("init", f_init);
        Swig_register_filebyname("director", f_directors);
        Swig_register_filebyname("director_h", f_directors_h);
        Swig_register_filebyname("director_helpers", f_directors_helpers);
        Swig_register_filebyname("initbeforefunc", f_initbeforefunc);

        modvar = 0;
        current = NO_CPP;

        Swig_banner(f_begin);

        Printf(f_runtime, "\n");
        Printf(f_runtime, "#define SWIGJSV8\n");
        Printf(f_runtime, "\n");

        /* Set module name */
        module = (Char(Getattr(n, "name")));
        // TODO(20): is this redundant?
        feature = (Char(Getattr(n, "name")));

        /* Start generating the initialization function */
        Printf(f_init, "\n", "#ifdef __cplusplus\n", "extern \"C\"\n", "#endif\n");
        Printv(f_init, "SWIGEXPORT void Init_", feature, "(void) {\n\n", NIL);

        Printf(f_init, "SWIG_InitializeModule(0);\n");

        Printf(f_init, "v8::Local<v8::Object> global = v8::Context::GetCurrent()->Global();\n\n", NIL);

        Language::top(n);

        /* Finish off our init function */
        Printf(f_init, "}\n");
        
        SwigType_emit_type_table(f_runtime, f_wrappers);

        /* Close all of the files */
        Dump(f_runtime, f_begin);
        Dump(f_header, f_begin);
        Dump(f_wrappers, f_begin);
        Dump(f_initbeforefunc, f_begin);
        Wrapper_pretty_print(f_init, f_begin);

        Delete(f_header);
        Delete(f_wrappers);
        Delete(f_init);
        Delete(f_initbeforefunc);
        Close(f_begin);
        Delete(f_runtime);
        Delete(f_begin);

        return SWIG_OK;
    }

    /* -----------------------------------------------------------------------------
    * importDirective()
    * ----------------------------------------------------------------------------- */
    virtual int importDirective(Node *n) {
        return Language::importDirective(n);
    }

    /* --------------------------------------------------------------------------
    * nativeWrapper()
    * -------------------------------------------------------------------------- */
    //TODO(012): what is this native thingie?
    virtual int nativeWrapper(Node *n) {
        return Language::nativeWrapper(n);
    }

    /* ---------------------------------------------------------------------
    * applyInputTypemap()
    *
    * Look up the appropriate "in" typemap for this parameter (p),
    * substitute the correct strings for the $target and $input typemap
    * parameters, and dump the resulting code to the wrapper file.
    * --------------------------------------------------------------------- */

    void applyInputTypemap(Parm *p, String *ln, String *source, Wrapper *f, String *symname) {
        String *tm;
        SwigType *pt = Getattr(p, "type");
        if ((tm = Getattr(p, "tmap:in"))) {
            Replaceall(tm, "$target", ln);
            Replaceall(tm, "$source", source);
            Replaceall(tm, "$input", source);
            Replaceall(tm, "$symname", symname);

            if (Getattr(p, "wrap:disown") || (Getattr(p, "tmap:in:disown"))) {
                Replaceall(tm, "$disown", "SWIG_POINTER_DISOWN");
            } else {
                Replaceall(tm, "$disown", "0");
            }

            Setattr(p, "emit:input", Copy(source));
            Printf(f->code, "%s\n", tm);
        } else {
            Swig_warning(WARN_TYPEMAP_IN_UNDEF, input_file, line_number, "Unable to use type %s as a function argument.\n", SwigType_str(pt, 0));
        }
    }

    Parm *skipIgnoredArgs(Parm *p) {
        while (checkAttribute(p, "tmap:in:numinputs", "0")) {
            p = Getattr(p, "tmap:in:next");
        }
        return p;
    }

    ///* ---------------------------------------------------------------------
    //* insertCleanupCode(ParmList *l, String *cleanup)
    //*
    //* Checks each of the parameters in the parameter list for a "freearg"
    //* typemap and (if it finds one) inserts the typemapping code into
    //* the function wrapper.
    //* --------------------------------------------------------------------- */

    //void insertCleanupCode(ParmList *l, String *cleanup) {
    //    String *tm;
    //    for (Parm *p = l; p;) {
    //        if ((tm = Getattr(p, "tmap:freearg"))) {
    //            if (Len(tm) != 0) {
    //                Replaceall(tm, "$source", Getattr(p, "lname"));
    //                Printv(cleanup, tm, "\n", NIL);
    //            }
    //            p = Getattr(p, "tmap:freearg:next");
    //        } else {
    //            p = nextSibling(p);
    //        }
    //    }
    //}

    void insertFunctionDefinition(Node *node, Wrapper* wrapper) {
        String *iname = Getattr(node, "sym:name");
        String *wname = Swig_name_wrapper(iname);

        String *nodeType = Getattr(node, "nodeType");
        
        if(current == MEMBER_VAR) {
            if(EndsWith(wname, "_get")) {
                Printv(wrapper->def, "v8::Handle<v8::Value> ", wname, "(v8::Local<v8::String> property, const v8::AccessorInfo& info) {\n", NIL);
            } else {
                Printv(wrapper->def, "void ", wname, "(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::AccessorInfo& info) {\n", NIL);
            }
        } else {
            Printv(wrapper->def, "v8::Handle<v8::Value> ", wname, "(const v8::Arguments& args) {\n", NIL);
        }
        Printv(wrapper->def, tab4, "v8::HandleScope scope;\n", NIL);        
    }    
    
    void insertInputParamMarshalling(Node* n, Wrapper* wrapper) {
        ParmList *parms  = Getattr(n,"parms");
        Parm *p;
        
        p = parms;
        for (int i=0; p; p = nextSibling(p), i++) {
            String   *name  = Getattr(p,"name");            
            String   *value = Getattr(p,"value");
            String   *type = Getattr(p,"type");
            
            String *lvalue = NewString("");
            String *rvalue = NewString("");
                                    
            Printf(lvalue, "arg%d", i+1, NIL);
            
            if( Strcmp(name, "self") == 0 ) {
                SwigType *selfType = Copy(type);
                SwigType_del_pointer(selfType);
                
                if( current == MEMBER_VAR ) {
                    Printv(rvalue, "v8_unwrap_this_pointer<", selfType, ">(info.Holder())", NIL);
                } else {
                    Printv(rvalue, "v8_unwrap_this_pointer<", selfType, ">(args.Holder())", NIL);
                }
                Printv(wrapper->locals, lvalue, " = ", rvalue, ";\n", NIL);
            } else {            
                if( current == MEMBER_VAR ) {
                    Printf(rvalue, "value");
                } else if (current == CONSTRUCTOR) {
                    Printf(rvalue, "args[%d]", i);
                } else {
                    Printf(rvalue, "args[%d]", i-1);
                }
                applyInputTypemap(p, lvalue, rvalue, wrapper, Getattr(n, "name"));                        
            }
            
            Delete(lvalue);
            Delete(rvalue);
        }
    }

    void insertFunctionBody(Node *n, Wrapper* wrapper) {
        String *iname = Getattr(n, "sym:name");
        String *nodeType = Getattr(n, "nodeType");
        String *type = Getattr(n, "type");
        ParmList *parms  = Getattr(n,"parms");
                        
        if (current == CONSTRUCTOR_INITIALIZE) {
            Printf(wrapper->code, "self->SetInternalField(0, v8::External::New(result);\n");
        }
    }
    
    void insertFunctionReturn(Node *n, Wrapper* f, String* actioncode) {
    
        int is_memberset = EndsWith(Getattr(n, "sym:name"), "_set");
        ParmList *parms  = Getattr(n,"parms");
        
        // failing can happen on marshalling
        int nparams = ParmList_len(parms);
        int can_fail = ((current == MEMBER_VAR || current == MEMBER_FUNC) && nparams>1)
            || (current == CONSTRUCTOR && nparams>0);
        
        if(!is_memberset) {
            /* Return the function value */
            String* tm = NewString("");
            if ((tm = Swig_typemap_lookup_out("out", n, "result", f, actioncode))) {
                Replaceall(tm, "$self", "obj0");
                Replaceall(tm, "$source", "result");
                Replaceall(tm, "$target", "resultobj");
                Replaceall(tm, "$result", "resultobj");
                if (GetFlag(n, "feature:new")) {
                    Replaceall(tm, "$owner", "SWIG_POINTER_OWN");
                } else {
                    Replaceall(tm, "$owner", "0");
                }
            }
            Printf(f->code, "%s\n", tm);
            Delete(tm);

            Printv(f->code, "return scope.Close(resultobj);\n", NIL);
            
            // insert a 'fail'
            if(can_fail) {
                Printf(f->code, "fail:\n");
                Printf(f->code, "return v8::Undefined();\n");        
            }
            Printf(f->code, "}\n");
        } else {
            if(can_fail) {
                Printf(f->code, "fail:\n");
            }
            Printv(f->code, "return;\n", "}\n", NIL);        
        }        
    }
    
    /* ---------------------------------------------------------------------
    * functionWrapper()
    *
    * Create a function declaration and register it with the interpreter.
    * --------------------------------------------------------------------- */

    virtual int functionWrapper(Node *n) {

        String *name = Getattr(n, "name");
        String *iname = Getattr(n, "sym:name");
        SwigType *type   = Getattr(n,"type");
        ParmList *parms  = Getattr(n,"parms");
        Setattr(n, "wrap:name", iname);        
                
        int numarg = emit_num_arguments(parms);
        int numreq = emit_num_required(parms);
        int varargs = emit_isvarargs(parms);
        
        int is_memberset = EndsWith(Getattr(n, "sym:name"), "_set");
        
        Wrapper *wrapper = NewWrapper();
        
        /* create the functions wrappered name */
        String *wname = Swig_name_wrapper(iname);
        String *nodeType = Getattr(n, "nodeType");
        
        // Function header
        // ----------------
        insertFunctionDefinition(n, wrapper);
        
        // Input parameters        
        // ------------------        
        emit_parameter_variables(parms, wrapper);
        emit_attach_parmmaps(parms, wrapper);
        /* Insert argument output code */
        String *outarg = NewString("");
        String* tm;
        for (Parm* p = parms; p;) {
            if ((tm = Getattr(p, "tmap:argout"))) {
                Replaceall(tm, "$source", Getattr(p, "lname"));
                Replaceall(tm, "$target", "resultobj");
                Replaceall(tm, "$arg", Getattr(p, "emit:input"));
                Replaceall(tm, "$input", Getattr(p, "emit:input"));
                Printv(outarg, "//TEST", tm, "\n", NIL);
                p = Getattr(p, "tmap:argout:next");
            } else {                
                p = nextSibling(p);
            }
        }
        emit_return_variable(n, type, wrapper);
        Append(wrapper->code, outarg);
        Delete(outarg);
        
        
        
        // Extra local vars
        // ------------------
        insertInputParamMarshalling(n, wrapper);

        if(! is_memberset ) {
            Wrapper_add_local(wrapper, "resultobj", "VALUE resultobj");
        }

        // Function body
        // ---------------
        
        String *actioncode = emit_action(n);
        insertFunctionBody(n, wrapper);
        
        // Return value
        // -------------
        insertFunctionReturn(n, wrapper, actioncode);
        //Note: don't call this it seems that this is not allowed but done by Swig_typemap_lookup_out???
        //Append(wrapper->code, actioncode);
                
        /* Dump the function out */
        Wrapper_print(wrapper, f_wrappers);
        
        return SWIG_OK;
    }

    /* ---------------------------------------------------------------------
    * variableWrapper()
    * --------------------------------------------------------------------- */

    virtual int variableWrapper(Node *n) {
        char *iname = GetChar(n, "sym:name");
        String *getname = Swig_name_get(NSPACE_TODO, iname);
        String *getfname = Swig_name_wrapper(getname);
        Setattr(n, "wrap:name", getfname);
        return SWIG_OK;
    }


    /* ---------------------------------------------------------------------
    * constantWrapper()
    * --------------------------------------------------------------------- */

    virtual int constantWrapper(Node *n) {
        return Language::constantWrapper(n);
    }

    /* -----------------------------------------------------------------------------
    * classDeclaration() 
    *
    * Records information about classes---even classes that might be defined in
    * other modules referenced by %import.
    * ----------------------------------------------------------------------------- */

    virtual int classDeclaration(Node *n) {
        return Language::classDeclaration(n);
    }

    /* ----------------------------------------------------------------------
    * classHandler()
    * ---------------------------------------------------------------------- */

    virtual int classHandler(Node *n) {
    
        String *clazz_init = NewString("");
        String *name = Getattr(n, "name");
        String *abstract = Getattr(n, "abstract");
        char* abstract_ = Char(abstract);
        
        String* clazz_templ = NewString("");
        JSV8_GetClassTemplStr(n, clazz_templ);
                
        char* define_class_templ = "v8::Persistent<v8::FunctionTemplate> %s;\n";
        char* create_class_templ = "\n%s = v8_create_class_template(\"%s\");\n";
        char* set_client_data = "SWIGTYPE_p_%s->clientdata = reinterpret_cast<void*>(&%s);\n";
        
        Printf(f_wrappers, define_class_templ, clazz_templ);
        Printf(clazz_init, create_class_templ, clazz_templ, name);
        Printf(clazz_init, set_client_data, name, clazz_templ);
        
                
        Append(f_init, clazz_init);
                        
        Language::classHandler(n);

        char* add_class_symbol = "global->Set(v8::String::NewSymbol(\"%s\"), %s->GetFunction());\n";
        Printf(f_init, add_class_symbol, name, clazz_templ);
        
        Delete(clazz_templ);
        Delete(clazz_init);
        
        return SWIG_OK;
    }

    /* ----------------------------------------------------------------------
    * memberfunctionHandler()
    *
    * Method for adding C++ member function
    *
    * By default, we're going to create a function of the form :
    *
    *         Foo_bar(this,args)
    *
    * Where Foo is the classname, bar is the member name and the this pointer
    * is explicitly attached to the beginning.
    *
    * The renaming only applies to the member function part, not the full
    * classname.
    *
    * --------------------------------------------------------------------- */

    virtual int memberfunctionHandler(Node *n) {
        current = MEMBER_FUNC;
        Language::memberfunctionHandler(n);
        
        Node* clazz = parentNode(n);
        String* clazzName = Getattr(clazz, "name");
        String* name = Getattr(n, "name");
        String* iname = Getattr(n, "sym:name");
        String* type = Getattr(n, "type");

        String* memFuncInit = NewString("");
        String* clazz_templ = NewString("");
        JSV8_GetClassTemplStr(clazz, clazz_templ);

        String* wrapper_name = NewString("");
        char* method_wrapper = "_wrap_%s_%s";
        Printf(wrapper_name, method_wrapper, clazzName, name);
        
        char* add_class_method = "v8_add_class_method(%s, \"%s\", %s);\n";
        
        Printf(memFuncInit, add_class_method, clazz_templ, name, wrapper_name);
        
        Append(f_init, memFuncInit);
        
        Delete(memFuncInit);
        Delete(clazz_templ);
        Delete(wrapper_name);
        
        return SWIG_OK;
    }

    /* ---------------------------------------------------------------------
    * constructorHandler()
    *
    * Method for adding C++ member constructor
    * -------------------------------------------------------------------- */

    virtual int constructorHandler(Node *n) {
        current = CONSTRUCTOR;
        
        Language::constructorHandler(n);
        
        Node* clazz = parentNode(n);
        String* clazzName = Getattr(clazz, "name");
        String* name = Getattr(n, "name");
        String* iname = Getattr(n, "sym:name");
        
        String* ctorInit = NewString("");  
        
        String* class_templ = NewString("");
        JSV8_GetClassTemplStr(clazz, class_templ); 
                
        char* alloc_func_str = "_wrap_new_%s";
        String* alloc_func = NewString("");
        Printf(alloc_func, alloc_func_str, clazzName);

        char* set_ctor_handler = "v8_set_allocate_handler(%s, %s);\n";
        Printf(ctorInit, set_ctor_handler, class_templ, alloc_func);
        
        Delete(class_templ);
        Delete(alloc_func);
        
        Append(f_init, ctorInit);
        Delete(ctorInit);
        
        return SWIG_OK;
    }

    /* ---------------------------------------------------------------------
    * membervariableHandler()
    *
    * This creates a pair of functions to set/get the variable of a member.
    * -------------------------------------------------------------------- */

    virtual int membervariableHandler(Node *n) {
        current = MEMBER_VAR;
        
        String* varFuncInit = NewString("");
        
        Node* clazz = parentNode(n);
        String* clazzName = Getattr(clazz, "name");
        String* varName = Getattr(n, "name");
        String* iName = Getattr(n, "sym:name");
        String* wName = Swig_name_wrapper(iName);
        
        String* v8_class_template = NewString("");
        String* wName_get = NewString("");        
        String* wName_set = NewString("");

        //TODO(010): use the same method that is used to get the wrapper name for the
        //           getter and setter function handler                         
        char* swig_getter = "_wrap_%s_%s_get";
        char* swig_setter = "_wrap_%s_%s_set";
        Printf(wName_get, swig_getter, clazzName, varName);
        Printf(wName_set, swig_setter, clazzName, varName);
        
        // TODO(011): handle read_only by adding a throwing setter function
        char* v8_add_gettersetter = "v8_add_class_member_getters_setters(%s, \"%s\", %s, %s);\n";
        JSV8_GetClassTemplStr(clazz, v8_class_template);
        Printf(varFuncInit, v8_add_gettersetter, v8_class_template, varName, wName_get, wName_set);
        
        Delete(v8_class_template);
        Delete(wName_get);
        Delete(wName_set);
        
        char* varFuncInit_ = Char(varFuncInit);
        
        Append(f_init, varFuncInit);
        Delete(varFuncInit);
               
        return Language::membervariableHandler(n); 
    }

    String *runtimeCode() {

        String *s = NewString("");
        String *shead = Swig_include_sys("jsv8head.swg");

        if (!shead) {
            Printf(stderr, "*** Unable to open 'jsv8head.swg'\n");
        } else {
            Append(s, shead);
            Delete(shead);
        }
        String *serrors = Swig_include_sys("jsv8errors.swg");
        if (!serrors) {
            Printf(stderr, "*** Unable to open 'jsv8errors.swg'\n");
        } else {
            Append(s, serrors);
            Delete(serrors);
        }
        String *sapi = Swig_include_sys("jsv8api.swg");
        if (!sapi) {
            Printf(stderr, "*** Unable to open 'jsv8api.swg'\n");
        } else {
            Append(s, sapi);
            Delete(sapi);
        }
        String *srun = Swig_include_sys("jsv8run.swg");
        if (!srun) {
            Printf(stderr, "*** Unable to open 'jsv8run.swg'\n");
        } else {
            Append(s, srun);
            Delete(srun);
        }
        return s;
    }

    String *defaultExternalRuntimeFilename() {
        return NewString("swigjsv8run.h");
    }
    
    /* ---------------------------------------------------------------------
    * main()
    *
    * Parse command line options and initializes variables.
    * --------------------------------------------------------------------- */

    virtual void main(int argc, char *argv[]) {

        /* Set location of SWIG library */
        SWIG_library_directory("js/v8");

        /* Turn on cppcast mode */
        Preprocessor_define((DOH *) "SWIG_CPLUSPLUS_CAST", 0);

        /* Add a symbol to the parser for conditional compilation */
        Preprocessor_define("SWIGJSV8 1", 0);

        SWIG_typemap_lang("jsv8");
        SWIG_config_file("jsv8.swg");

        allow_overloading();
    }
    
};

/* -----------------------------------------------------------------------------
* swig_jsv8()    - Instantiate module
* ----------------------------------------------------------------------------- */

static Language *new_swig_js_v8() {
    return new JavaScriptV8();
}
extern "C" Language *swig_js_v8(void) {
    return new_swig_js_v8();
}
