#pragma once
#include "agui/ag_types.hpp"
#include <type_traits>

namespace agui {

class AgObj;
struct AgEvent {
    AgEvent(ag_uint8 class_) : class_(class_) {}

    const ag_uint32 class_{};
    ag_bool handled{ ag_false };
    NullablePtr<AgObj> sender;

    template<class T, std::enable_if_t<std::is_base_of_v<AgEvent, T>>* v = nullptr>
    T& As() {
        return *static_cast<T*>(this);
    }

    AgEvent& Handled() {
        handled = ag_true;
        return *this;
    }

    virtual ~AgEvent() = default;
};

}
