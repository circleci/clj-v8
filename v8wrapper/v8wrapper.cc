#include <v8.h>
#include <string.h>

#include "v8wrapper.h"

using namespace v8;

char *run(const char *jssrc)
{
    HandleScope handle_scope;
    Persistent<Context> context = Context::New();

    // Enter the created context for compiling and
    // running the script.
    Context::Scope context_scope(context);

    Handle<String> source = String::New(jssrc);
    Handle<Script> script = Script::Compile(source);

    // Run the script
    Handle<Value> result = script->Run();
    context.Dispose();

    // return result as string, must be deallocated in cgo wrapper
    String::AsciiValue ascii(result);

    return strdup(*ascii);
}
