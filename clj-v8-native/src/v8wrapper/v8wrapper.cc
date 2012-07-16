#include <cstdlib>
#include <iostream>
#include <wchar.h>
#include <errno.h>
#include <v8.h>
#include <string.h>
#include "v8wrapper.h"

using namespace v8;

wchar_t *fromValue(Handle<Value> v) {
  String::Value ucs2str(v);
  
  wchar_t *result = (wchar_t*) calloc(sizeof(wchar_t), ucs2str.length() + 1);
  
  wchar_t *dst = result;
  uint16_t *src = *ucs2str;

  while (*src) {
    *dst++ = *src++;
  }
  
  return result;
}


wchar_t *run(wchar_t *jssrc) {
  HandleScope handle_scope;
  Persistent<Context> context = Context::New();
  Context::Scope context_scope(context);

  uint16_t *jsu16 = (uint16_t*) calloc(sizeof(uint16_t), wcslen(jssrc));

  // Copy input parameter
  {
    uint16_t *dst = jsu16;
    const wchar_t *src = jssrc;

    int count = 0;
    
    while (*src) {
      *dst++ = *src++;
      count++;
    }
  }

  Handle<Script> script = Script::Compile(String::New(jsu16));

  // After compiling the script, free the temporary buffer we used to load
  // the script from wchar_t to uint16_t or we'll leak it.
  free(jsu16);

  if (*script != NULL) {
    TryCatch trycatch;
    
    Handle<Value> v = script->Run();

    if (v.IsEmpty()) {
      Handle<Value> exception = trycatch.Exception();

      wchar_t *result = fromValue(exception);

      // magic vs more magic
      context.Dispose();
      errno = -69;

      return result;
      
    } else {
      wchar_t *result = fromValue(v);

      context.Dispose();
      errno = 0;
      
      return result;
    }
  }

  return NULL;
}


int cleanup(void *lastresult) {
  free(lastresult);

  return 0;
}
