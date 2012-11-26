#include <cstdlib>
#include <iostream>
#include <wchar.h>
#include <errno.h>
#include <v8.h>
#include <string.h>
#include "v8wrapper.h"

using namespace v8;
using namespace std;

Handle<String> ReadFile(const char* name);
const char* ToCString(const String::Utf8Value& value);
wchar_t *val2wchar(const Handle<Value> v);
uint16_t* wchar2uint16(wchar_t* w);

Handle<Value> Print(Handle<Value>& arg) {
  String::Utf8Value str(arg);
  const char* cstr = ToCString(str);
  printf("%s", cstr);

  printf("\n");
  fflush(stdout);
  return v8::Undefined();
}

int main()
{
  HandleScope handle_scope;
  Handle<String> source = ReadFile("test_file.js");

  wchar_t* result = run(val2wchar(source));
  Handle<Value> resstr = String::New(wchar2uint16(result));
  Print(resstr);
}
