#include <cstdlib>
#include <iostream>
#include <wchar.h>
#include <errno.h>
#include <v8.h>
#include <string.h>
#include <cassert>
#include "v8wrapper.h"

using namespace v8;

const char* ToCString(const String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

void ReportException(v8::TryCatch* try_catch) {
  v8::HandleScope handle_scope;
  v8::String::Utf8Value exception(try_catch->Exception());
  const char* exception_string = ToCString(exception);
  v8::Handle<v8::Message> message = try_catch->Message();
  if (message.IsEmpty()) {
    // V8 didn't provide any extra information about this error; just
    // print the exception.
    fprintf(stderr, "%s\n", exception_string);
  } else {
    // Print (filename):(line number): (message).
    v8::String::Utf8Value filename(message->GetScriptResourceName());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber();
    fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
    // Print line of source code.
    v8::String::Utf8Value sourceline(message->GetSourceLine());
    const char* sourceline_string = ToCString(sourceline);
    fprintf(stderr, "%s\n", sourceline_string);
    // Print wavy underline (GetUnderline is deprecated).
    int start = message->GetStartColumn();
    for (int i = 0; i < start; i++) {
      fprintf(stderr, " ");
    }
    int end = message->GetEndColumn();
    for (int i = start; i < end; i++) {
      fprintf(stderr, "^");
    }
    fprintf(stderr, "\n");
    v8::String::Utf8Value stack_trace(try_catch->StackTrace());
    if (stack_trace.length() > 0) {
      const char* stack_trace_string = ToCString(stack_trace);
      fprintf(stderr, "%s\n", stack_trace_string);
    }
  }
}

Handle<String> ReadFile(const char* name) {
  FILE* file = fopen(name, "rb");
  if (file == NULL) return Handle<String>();

  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  rewind(file);

  char* chars = new char[size + 1];
  chars[size] = '\0';
  for (int i = 0; i < size;) {
    int read = static_cast<int>(fread(&chars[i], 1, size - i, file));
    i += read;
  }
  fclose(file);
  Handle<String> result = String::New(chars, size);
  delete[] chars;
  return result;
}

// The callback that is invoked by v8 whenever the JavaScript 'read'
// function is called.  This function loads the content of the file named in
// the argument into a JavaScript string.
v8::Handle<v8::Value> Read(const v8::Arguments& args) {
  if (args.Length() != 1) {
    return v8::ThrowException(v8::String::New("Bad parameters"));
  }
  v8::String::Utf8Value file(args[0]);
  if (*file == NULL) {
    return v8::ThrowException(v8::String::New("Error loading file"));
  }
  v8::Handle<v8::String> source = ReadFile(*file);
  if (source.IsEmpty()) {
    return v8::ThrowException(v8::String::New("Error loading file"));
  }
  return source;
}



wchar_t *val2wchar(const Handle<Value> v) {
  String::Value ucsstr(v);

  int length = ucsstr.length() + 1;
  wchar_t *result = (wchar_t*) calloc(sizeof(wchar_t), length);

  for(int i = 0; i < length; i++) {
    result[i] = (*ucsstr)[i];
  }

  return result;
}

Handle<String> wchar2v8string(wchar_t* w) {
  int length = wcslen(w);
  uint16_t *u16 = (uint16_t*) calloc(length+1, sizeof(uint16_t));

  for(int i = 0; i < length; i++) {
    u16[i] = w[i];
  }

  Handle<String> result = String::New(u16);
  free(u16);

  return result;
}



Handle<Value> run_scoped(Persistent<Context> context, Handle<String> src) {
  Context::Scope context_scope(context);
  v8::HandleScope handle_scope;

  TryCatch trycatch;
  Handle<Value> result;

  Handle<Script> script = Script::Compile(src);
  if (*script == NULL) { // parse/compile failure
    result = trycatch.Exception();
    ReportException(&trycatch);
    errno = -1;
  } else {
    result = script->Run();
    errno = 0;

    if (result.IsEmpty()) { // canonical way to test for exceptions
      result = trycatch.Exception();
      ReportException(&trycatch);
      errno = -69;
    }
  }

  return result;
}

Persistent<Context> createContext ()
{
  v8::HandleScope handle_scope;
  Local<ObjectTemplate> global;

  // add readFile
  global = ObjectTemplate::New();
  global->Set(String::New("readFile"), FunctionTemplate::New(Read));

  // init context
  return Context::New(NULL, global);
}

struct _v8stack {
  v8::Isolate* isolate;
};
typedef struct _v8stack v8stack;

v8stack* createNewContext() {
  v8stack* stack = new v8stack;
  stack->isolate = v8::Isolate::New();
}


wchar_t *run(wchar_t *jssrc) {

  // convert input
  wchar_t* result_str = NULL;

  v8::Isolate* isolate = v8::Isolate::New();
  {
     v8::Locker locker(isolate);
     v8::Isolate::Scope iso_scope(isolate);

     // run
     Persistent<Context> context = createContext();
     {
       v8::HandleScope handle_scope;
       Handle<String> src = wchar2v8string(jssrc);
       Handle<Value> result = run_scoped(context, src);
       result_str = val2wchar(result);
     }
     context.Dispose();
  }
  isolate->Dispose();

  return result_str;
}


int cleanup(void *lastresult) {
  free(lastresult);

  return 0;
}
