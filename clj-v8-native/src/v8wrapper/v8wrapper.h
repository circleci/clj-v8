#ifndef _V8WRAPPER_H
#define _V8WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

  // single object to hold values in Java-land
  typedef struct v8_tuple_s v8_tuple;

  v8_tuple* CreateTuple();
  int CleanupTuple(v8_tuple* tuple);

  // compiles and executes JavaScript and returns the script return value as string
  wchar_t* Run(v8_tuple*, wchar_t* source, wchar_t* name);
  
  void InitializeICU();

  void SetFlags(wchar_t* flags);

  // free memory returned from the last run call
  int Cleanup(void* last_result);

#ifdef __cplusplus
}
#endif

#endif // _V8WRAPPER_H
