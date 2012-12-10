#ifndef _V8WRAPPER_H
#define _V8WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

  /* Single object to hold values in Java-land */
  struct _v8tuple;
  typedef struct _v8tuple v8tuple;

  v8tuple* create_tuple();
  int cleanup_tuple(v8tuple* tuple);

  // compiles and executes javascript and returns the script return value as string
  wchar_t *run(v8tuple*, wchar_t *js);

  // free memory returned from the last run call
  int cleanup(void *lastresult);


#ifdef __cplusplus
}
#endif

#endif // _V8WRAPPER_H
