#include <v8.h>

#include <iostream>
#include <string>

extern void Init_example(void);

using namespace v8;

void test1() {
    HandleScope handle_scope;

    Persistent<v8::Context> context = v8::Context::New();
    v8::Context::Scope context_scope(context);

    Init_example();

    //        Handle<String> source = String::New("var bla = new Bla(); bla.bla(); bla.blupp(2);");
//    Handle<String> source = String::New("var circle = new Circle(2.0); log('Hallo Welt!'); log(circle.area());");
    Handle<String> source = String::New("var circle = new Circle(2.0); circle.x = 1.2;");
    // Compile the source code. 
    Handle<Script> script = Script::Compile(source);

    // Run the script to get the result. 
    Handle<Value> result = script->Run();

    context.Dispose();
}


int main(int argc, char* argv[]) {
    
    test1();    
    return 0;
}
