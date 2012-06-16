#include <cstdlib>
#include <wchar.h>

#include <v8.h>
#include <string.h>

#include "v8wrapper.h"

using namespace v8;

const wchar_t *run(const wchar_t *jssrc)
{
  TryCatch try_catch;
  HandleScope handle_scope;

  Persistent<Context> context = Context::New();
  Context::Scope context_scope(context);

  uint16_t *jsu16 = (uint16_t*) calloc(sizeof(uint16_t), wcslen(jssrc));

  // Copy input parameter
  {
    uint16_t *dst = jsu16;
    const wchar_t *src = jssrc;

    while (*dst++ = *src++) {}
  }

  Handle<Script> script = Script::Compile(String::New(jsu16));

  if (*script != NULL) {
    Handle<Value> v = script->Run();

    String::Value ucs2str(v);
    wchar_t *result = (wchar_t*) calloc(sizeof(wchar_t), ucs2str.length() + 1);
    wchar_t *dst = result;
    uint16_t *src = *ucs2str;

    while (*dst++ = *src++) {
      // Nada!
    }

    context.Dispose();

    return result;
  }

  return NULL;
}
