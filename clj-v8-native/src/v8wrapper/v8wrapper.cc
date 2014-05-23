#include <cassert>
#include <cstdlib>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <v8.h>
#include <wchar.h>

#include "v8wrapper.h"

const char* ToCString(const v8::String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

void ReportException(v8::Isolate* isolate, v8::TryCatch* try_catch) {
  v8::HandleScope handle_scope(isolate);
  v8::String::Utf8Value exception(try_catch->Exception());
  const char* exception_string = ToCString(exception);
  v8::Handle<v8::Message> message = try_catch->Message();
  if (message.IsEmpty()) {
    // V8 didn't provide any extra information about this error; just
    // print the exception.
    printf("%s\n", exception_string);
  } else {
    // Print (filename):(line number): (message).
    v8::String::Utf8Value filename(message->GetScriptResourceName());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber();
    printf("%s:%i: %s\n", filename_string, linenum, exception_string);
    // Print line of source code.
    v8::String::Utf8Value sourceline(message->GetSourceLine());
    const char* sourceline_string = ToCString(sourceline);
    printf("%s\n", sourceline_string);
    // Print wavy underline (GetUnderline is deprecated).
    int start = message->GetStartColumn();
    for (int i = 0; i < start; i++) {
      printf(" ");
    }
    int end = message->GetEndColumn();
    for (int i = start; i < end; i++) {
      printf("^");
    }
    printf("\n");
  }
}

v8::Handle<v8::String> ReadFile(v8::Isolate* isolate, const char* filename) {
  FILE* file = fopen(filename, "rb");
  
  if (file == NULL) {
    return v8::Handle<v8::String>(); }

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
  
  v8::Handle<v8::String> result =
      v8::String::NewFromUtf8(isolate, chars, v8::String::kNormalString, size);
  
  delete[] chars;
  
  return result;
}

// The callback that is invoked by v8 whenever the JavaScript 'read' function is
// called. Loads content of the file named in the argument into a JavaScript
// string.
void Read(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  
  if (args.Length() != 1) {
    isolate->ThrowException(
      v8::String::NewFromUtf8(isolate, "Too many arguments"));
    return;
  }

  v8::String::Utf8Value filename(args[0]);
  
  if (*filename == NULL) {
    isolate->ThrowException(
      v8::String::NewFromUtf8(isolate, "Bad filename"));
    return;
  }
  
  v8::Handle<v8::String> content = ReadFile(isolate, *filename);
  
  if (content.IsEmpty()) {
    isolate->ThrowException(
      v8::String::NewFromUtf8(isolate, "Error reading file (empty?)"));
    return;
  }

  args.GetReturnValue().Set(content);
}

wchar_t* ValueToWChar(const v8::Handle<v8::Value> val) {
  v8::String::Utf8Value vals(val);
  const char* cs = *vals;
  int length = strlen(cs) + 1;
  wchar_t* wcs = new wchar_t[length];

  // convert to wchar_t
  mbstowcs(wcs, cs, length);
  
  return wcs;
}

v8::Handle<v8::String> WCharToString(v8::Isolate* isolate, wchar_t* w) {
  int length = wcslen(w);
  int i;
  uint16_t* buf = (uint16_t*) calloc(length + 1, sizeof(uint16_t));
  
  for (i = 0; i < length; i++) {
    buf[i] = w[i];
  }
  
  v8::Handle<v8::String> result =
    v8::String::NewFromTwoByte(isolate, buf, v8::String::kNormalString, length);

  free(buf);
  
  return result;
}

v8::Handle<v8::Value> RunInSameScope(v8::Isolate* isolate,
				     v8::Handle<v8::String> source,
				     v8::Handle<v8::String> name)
{
  // can create new scope, but must change return type to persistent
  // v8::HandleScope handle_scope(isolate);

  v8::ScriptOrigin origin(name);
  v8::TryCatch try_catch;
  v8::Handle<v8::Script> script = v8::Script::Compile(source, &origin);
  v8::Handle<v8::Value> result;
  
  if (script.IsEmpty()) {
    result = try_catch.Exception();
    ReportException(isolate, &try_catch);
    errno = -1;
  } else {
    result = script->Run();
    errno = 0;

    if (try_catch.HasCaught()) {
      result = try_catch.Exception();
      ReportException(isolate, &try_catch);
      errno = -69;
    }
  }

  return result;
}

v8::Persistent< v8::Context, v8::CopyablePersistentTraits<v8::Context> >
CreateContext(v8::Isolate* isolate)
{
  v8::Locker locker(isolate);
  v8::Isolate::Scope isolate_scope(isolate);
  v8::HandleScope handle_scope(isolate);

  v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

  // bind the global 'readFile'
  global->Set(v8::String::NewFromUtf8(isolate, "readFile"),
	      v8::FunctionTemplate::New(isolate, Read));

  // create context
  v8::Handle<v8::Context> context = v8::Context::New(isolate, NULL, global);

  // make it persist outside this scope
  // see: http://stackoverflow.com/questions/22646546
  v8::Persistent< v8::Context, v8::CopyablePersistentTraits<v8::Context> >
    persistent_context(isolate, context);

  return persistent_context;
}

struct v8_tuple_s {
  v8::Isolate* isolate;
  v8::Persistent< v8::Context, v8::CopyablePersistentTraits<v8::Context> > context;
};

v8_tuple* CreateTuple() {
  v8::Isolate* isolate = v8::Isolate::New();

  v8::Persistent< v8::Context, v8::CopyablePersistentTraits<v8::Context> >
    context = CreateContext(isolate);
  
  v8_tuple* tuple = new v8_tuple;
  
  tuple->isolate = isolate;
  tuple->context = context;
  
  return tuple;
}

wchar_t* Run(v8_tuple* tuple, wchar_t* source, wchar_t* name) {
  wchar_t* result = NULL;

  // acquire isolate
  v8::Locker locker(tuple->isolate);
  v8::Isolate::Scope isolate_scope(tuple->isolate);
  v8::HandleScope handle_scope(tuple->isolate);

  v8::Local<v8::Context> context
    = v8::Local<v8::Context>::New(tuple->isolate, tuple->context);
  
  // enter the execution environment  
  v8::Context::Scope context_scope(context);

  v8::Handle<v8::String> source_ = WCharToString(tuple->isolate, source);
  v8::Handle<v8::String> name_ = WCharToString(tuple->isolate, name);
  
  // run the script
  v8::Handle<v8::Value> result_ = RunInSameScope(tuple->isolate, source_, name_);
  result = ValueToWChar(result_);
  
  return result;
}

int CleanupTuple(v8_tuple* tuple) {
  {
    v8::Locker locker(tuple->isolate);
    v8::Isolate::Scope isolate_scope(tuple->isolate);
  }

  tuple->isolate->Dispose();
  free(tuple);
  
  return 0;
}

void InitializeICU() {
  v8::V8::InitializeICU();
}

void SetFlags(wchar_t* flags) {
  unsigned int length = wcslen(flags) * (sizeof(char) / sizeof(wchar_t));
  char buf[length];
  wcstombs(buf, flags, (size_t) length);
  
  v8::V8::SetFlagsFromString(buf, length);
}

int Cleanup(void* last_result) {
  free(last_result);
  return 0;
}
