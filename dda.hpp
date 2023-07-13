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
