/*
 * \file ag_ref_obj.h
 * \brief 引用保护
*/

#pragma once
#include "agui/ag_types.hpp"

namespace agui {

class AgMasterRefObj {
public:
    AgMasterRefObj();
    ~AgMasterRefObj();
private:
    friend class AgSlaveRefObj;
    void* ptr{};
};

class AgSlaveRefObj {
public:
    AgSlaveRefObj(AgMasterRefObj& master);
    ~AgSlaveRefObj();

    ag_bool IsMasterDead() const;
private:
    void* ptr{};
};

}
