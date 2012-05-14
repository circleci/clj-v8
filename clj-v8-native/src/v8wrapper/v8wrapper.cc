#include <v8.h>
#include <string.h>

#include "v8wrapper.h"

using namespace v8;

char *run(const char *jssrc)
{
  TryCatch try_catch;
  HandleScope handle_scope;

  Persistent<Context> context = Context::New();
  Context::Scope context_scope(context);

  Handle<Script> script = Script::Compile(String::New(jssrc));
  Handle<Value> v = script->Run();
  char* result;

  if (v.IsEmpty())
  {
    String::AsciiValue ascii(v);
    result = strdup(*ascii);
  } else {
    Handle<Value> exception = try_catch.Exception();
    String::AsciiValue exception_str(exception);
    result = strdup(*exception_str);
  }

  context.Dispose();

  return result;
}
