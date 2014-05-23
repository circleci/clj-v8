#include <boost/thread.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <wchar.h>

#include "v8wrapper.h"

// returns size, chars as an output parameter
long read(const char* name, char** result) {
  FILE* file = fopen(name, "rb");
  if (file == NULL) return -1;

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
  *result = chars;
  return size;
}

void run_v8() {
  // read file
  char* source8 = NULL;
  long size = read("test_file.js", &source8);
  if (source8 == NULL or size == -1) {
    return;
  }
  
  // convert to 16 bit chars
  wchar_t* source16 = (wchar_t*)(calloc(size+1, 4));
  for (int i = 0; i < size; i++) {
    source16[i] = (wchar_t)(source8[i]);
  }
  free(source8);

  // get a tuple to use
  v8_tuple* tuple = CreateTuple();
  
  wchar_t name[32];
  swprintf(name, 32, L"%s", "unknown");

  // run test
  wchar_t* result = Run(tuple, source16, name);
  free(source16);

  wprintf(L"%ls", result);
  
  CleanupTuple(tuple);
  Cleanup(result);
}

int main() {
  int count = 20;

  boost::thread ts[count];

  for (int i = 0; i < count; i++) {
    wprintf(L"starting thread %i\n", i);
    ts[i] = boost::thread(run_v8);
  }

  for (int i = 0; i < count; i++) {
    ts[i].join();
    wprintf(L"ending thread %i\n", i);
  }

  return 0;
}
