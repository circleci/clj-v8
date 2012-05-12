#ifndef _V8WRAPPER_H
#define _V8WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif
    // compiles and executes javascript and returns the script return value as string
    char *run(const char *js);

#ifdef __cplusplus
}
#endif

#endif // _V8WRAPPER_H
