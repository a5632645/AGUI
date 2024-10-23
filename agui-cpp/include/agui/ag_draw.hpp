#pragma once
#include "agui/ag_types.hpp"
#include <type_traits>

namespace agui {

struct AgDraw {
    ag_uint32 type;

    explicit AgDraw(ag_uint32 type) : type(type) {}

    template<class T, std::enable_if_t<std::is_base_of_v<AgDraw, T>>* v = nullptr>
    T& As() {
        return *static_cast<T*>(this);
    }

    template<class T, std::enable_if_t<std::is_base_of_v<AgDraw, T>>* v = nullptr>
    const T& As() const {
        return *static_cast<const T*>(this);
    }
};

}
