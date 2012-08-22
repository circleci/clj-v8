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


wchar_t *run_scoped(Persistent<Context> context, uint16_t *src) {
  HandleScope handle_scope;  
  Context::Scope context_scope(context);
  
  Handle<Script> script = Script::Compile(String::New(src));

  Handle<Value> executionresult;
  wchar_t *result = NULL;

  if (*script != NULL) {
    TryCatch trycatch;
    executionresult = script->Run();

    if (!executionresult.IsEmpty()) {
      result = fromValue(executionresult);
      errno = 0;

    } else {
      executionresult = trycatch.Exception();
      result = fromValue(executionresult);

      errno = -69;
    }
  }

  return result;
}


wchar_t *run(wchar_t *jssrc) {
  Persistent<Context> context = Context::New();

  uint16_t *jsu16 = (uint16_t*) calloc(sizeof(uint16_t), wcslen(jssrc) + 1);

  // Copy input parameter
  {
    uint16_t *dst = jsu16;
    const wchar_t *src = jssrc;

    int count = 0;
    
    while (*dst++ = *src++) {
      count++;
    }
  }

  wchar_t *result = run_scoped(context, jsu16);
  context.Dispose();
  
  // After compiling the script, free the temporary buffer we used to load
  // the script from wchar_t to uint16_t or we'll leak it.
  free(jsu16);

  return result;
}


int cleanup(void *lastresult) {
  free(lastresult);

  return 0;
}

