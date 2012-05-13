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

  if (v.isEmpty())
  {
    String::Utf8Value utf8(v);
    result = strdup(*v);
  } else {
    result = strdup("Fail");
  }

  context.Dispose();
  V8::dispose();

  return result;
}
