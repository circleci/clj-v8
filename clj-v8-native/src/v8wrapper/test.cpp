#include <cstdlib>
#include <iostream>
#include <wchar.h>
#include <errno.h>
#include <v8.h>
#include <string.h>
#include "v8wrapper.h"
#include <boost/thread.hpp>


v8::Handle<v8::String> ReadFile(const char* name);
const char* ToCString(const v8::String::Utf8Value& value);
wchar_t *val2wchar(const v8::Handle<v8::Value> v);
uint16_t* wchar2uint16(wchar_t* w);

v8::Handle<v8::Value> Print(v8::Handle<v8::Value>& arg) {
  v8::String::Utf8Value str(arg);
  const char* cstr = ToCString(str);
  printf("%s", cstr);

  printf("\n");
  fflush(stdout);
  return v8::Undefined();
}

void run_v8() {
  v8::HandleScope handle_scope;

  v8::Handle<v8::String> source = ReadFile("test_file.js");

  wchar_t* result = run(val2wchar(source));
  v8::Handle<v8::Value> resstr = v8::String::New(wchar2uint16(result));
}

int main()
{
  int count = 20;
  v8::V8::Initialize();

  boost::thread ts[count];

  for (int i = 0; i < count; i++) {
    printf("starting thread %i\n", i);
    ts[i] = boost::thread(run_v8);
  }

  for (int i = 0; i < count; i++) {
    ts[i].join();
    printf("ending thread %i\n", i);
  }
}
