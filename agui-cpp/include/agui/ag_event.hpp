#pragma once
#include "agui/ag_types.hpp"
#include <type_traits>

namespace agui {

class AgObj;
class AgEvent {
public:
    const ag_uint8 class_{};
    const ag_uint8 id{};
    ag_bool handled{};
    NullablePtr<AgObj> sender;

    template<class T, std::enable_if_t<std::is_base_of_v<AgObj, T>>* v = nullptr>
    T& As() {
        return *static_cast<T*>(this);
    }

    virtual ~AgEvent() = default;
};

}
