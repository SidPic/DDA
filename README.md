compiled with -std=c++23

This header file implements the Allocator<Base> class
for automatic allocation of memory
for objects derived of the <Base> class at runtime,
ensuring the unity of the returned type.

Memory management of child objects is performed using Allocator<Base>::Handler,
the standard implementation of which is represented
by the template variable StdHandler<Base,Derived>.

Example:
```cpp
include <cstdio>
include "dda.hpp"

using namespace dda;

class A {public: virtual void hello() { puts("I'm \'A\'"); } };
class B : public A {public: virtual void hello() { puts("I'm \B\'"); } };
class C : public B {public: virtual void hello() { puts("I'm \'C\'"); } };

int main()
{
    Allocator<A> A_alloc(StdHandler<A,C>);
    A* object;

    object = A_alloc.allocate();
    object->hello(); // prints "I'm C"
    A_alloc.deallocate(object);
    // or
    A_alloc.flush(); // deallocate all allocated data
    object->hello(); // Segmentation fault

    return 0;
}
```
