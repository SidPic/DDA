// Simple implementation :D
#include <map>

namespace dda { std::map<std::string, void*(*)()> alloc; }

#define DDA_addalloc(classname) dda::alloc.emplace(#classname,[]()->void*{return new classname;})

/*------------------------ demo -----------------------------*/

#include <iostream>

struct A {
    virtual void hello () { std::cout << "I'm A" << std::endl; }
};

struct B : A {
    virtual void hello () { std::cout << "I'm B" << std::endl; }
};


int main()
{
    DDA_addalloc(A);
    DDA_addalloc(B);

    A* a;

    a = (A*)dda::alloc["B"]();
    a->hello();

    delete a;
    a->hello(); // Segmentation fault

    return 0;
}
