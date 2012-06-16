#include <cstdlib>
#include <wchar.h>

#include <v8.h>
#include <string.h>

#include "v8wrapper.h"

using namespace v8;

const wchar_t *run(const char *jssrc)
{
  TryCatch try_catch;
  HandleScope handle_scope;

  Persistent<Context> context = Context::New();
  Context::Scope context_scope(context);

  Handle<Script> script = Script::Compile(String::New(jssrc));
  Handle<Value> v = script->Run();

  //  if (v.IsEmpty()) {
  String::Value ucs2str(v);
  //uint16_t *result = (uint16_t*) L"Dicks";
  wchar_t *result = (wchar_t*) calloc(sizeof(wchar_t), ucs2str.length() + 1);

  wchar_t *dest = result;
  uint16_t *src = *ucs2str;

  while (*dest++ = *src++) {
    // Nada!
  }

  /*  } else {
      Handle<Value> exception = try_catch.Exception();
      String::Utf8Value exception_str(exception);
      result = strdup(*exception_str);
      }*/

  context.Dispose();

  return result;
}
