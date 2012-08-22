#include <wchar.h>
#include "v8wrapper.h"


int main(void) {
  wchar_t foo[] = L"function addOne(x) { return x + 1; }; addOne(3);";

  //while (1) {
    wchar_t *result = run(foo);
    printf("%ls\n", result);
    cleanup(result);
    //}

  return 0;
}

