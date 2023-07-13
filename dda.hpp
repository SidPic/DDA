/*
 * compiled with -std=c++23
 *
 * This header file implements the Allocator<Base> class
 * for automatic allocation of memory
 * for objects derived of the <Base> class at runtime,
 * ensuring the unity of the returned type.
 *
 * Memory management of child objects is performed using Allocator<Base>::Handler,
 * the standard implementation of which is represented
 * by the template variable StdHandler<Base,Derived>.
 *
 * Example:
 *
   #include <cstdio>
   #include "dda.hpp"

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
 *
 * */

#ifndef DYNAMIC_DERIVED_ALLOCATOR_HPP
#define DYNAMIC_DERIVED_ALLOCATOR_HPP

#include <vector>

namespace dda {
//////////////////////////////////////////

inline void* to_deallocate;

enum class Command { New, Delete, Flush };

template <class Base>
class Allocator {
public:

    typedef Base* (*Handler) (Command);

    Handler handle;

    constexpr Allocator(Handler handler):
        handle(handler)
    { }

    constexpr Base* allocate () {
        return handle(Command::New);
    }

    constexpr void deallocate (Base* ptr) {
        to_deallocate = ptr;
        handle(Command::Delete);
    }

    constexpr void flush () {
        handle(Command::Flush);
    }

    ~Allocator() {
        flush();
    }
};

template <class Base, class Derived>
requires (std::is_base_of<Base,Derived>::value)
inline constexpr Allocator<Base>::Handler StdHandler = [](Command cmd) -> Base*
{
    static std::vector<Derived> data;

    switch (cmd) {

    case Command::New:
        return &data.emplace_back();

    case Command::Delete:
        data.erase(typename decltype(data)::iterator((Derived*)to_deallocate));
        break;

    case Command::Flush:
        data.clear();
        break;

    } return nullptr;
};

//////////////////////////////////////////
}   /* namespace dda */
#endif /* DYNAMIC_DERIVED_ALLOCATOR_HPP */
