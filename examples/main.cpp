#include "function_objects.h"

// Example usage:
void vv(FunctionObject<void()>);
void vi(FunctionObject<void(int)>);
int iiii(FunctionObject<int(int, int, int)>);

static void g() {}

int demonstration()
{
  // Non-capturing lambda, void()
  vv([]{});

  // Capturing lambda, void()
  int x = 3;
  vv([&]{x++;}); // x == 4

  // Free function, void()
  // both pointer to function and reference to function are valid
  vv(&g);
  vv(g);

  // std::function, void(); commented out because it wrecks codegen
  //   std::function<void()> f = [&]{ x += 21; };
  //   vv(f);

  vi([&](int i){x += i;}); // x == 11

  // x == 11 + 3 + 4 * 5 == 34
  return iiii([&](int a, int b, int c) { return x + a + b * c; });
}

/*
By defining vv(), vi() and iiii() as follows, clang and gcc are able to inline
and simplify all code in demonstration(). Mark those functions static for even
less generated code.

demonstration():
        mov     eax, 34
        ret
*/

void vv(FunctionObject<void()> f)
{
  f();
}

void vi(FunctionObject<void(int)> f)
{
  f(7);
}

int iiii(FunctionObject<int(int, int, int)> f)
{
  return f(3, 4, 5);
}

int main() {
    return demonstration();
}

