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

    AgMasterRefObj(const AgMasterRefObj&) = delete;
    AgMasterRefObj& operator=(const AgMasterRefObj&) = delete;
    AgMasterRefObj(AgMasterRefObj&&) noexcept;
    AgMasterRefObj& operator=(AgMasterRefObj&&) noexcept;
private:
    void DestroyMaster();

    friend class AgSlaveRefObj;
    void* ptr{};
};

class AgSlaveRefObj {
public:
    AgSlaveRefObj(AgMasterRefObj& master);
    ~AgSlaveRefObj();

    AgSlaveRefObj(const AgSlaveRefObj&) = delete;
    AgSlaveRefObj& operator=(const AgSlaveRefObj&) = delete;
    AgSlaveRefObj(AgSlaveRefObj&&) noexcept;
    AgSlaveRefObj& operator=(AgSlaveRefObj&&) noexcept;

    ag_bool IsMasterDead() const;
private:
    void DestroySlave();

    void* ptr{};
};

}
