//
// Created by toor on 11/14/23.
//

#ifndef ATOM_MACROS_H
#define ATOM_MACROS_H
#include "ATOM/atompch.h"


namespace Atom {

    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }



    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}



#endif //ATOM_MACROS_H
