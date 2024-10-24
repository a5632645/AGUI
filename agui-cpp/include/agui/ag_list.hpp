#pragma once
#include "agui/ag_types.hpp"
#include <type_traits>

namespace agui {

class AgListNode {
public:
    virtual ~AgListNode() = default;

    template<class T, std::enable_if_t<std::is_base_of_v<AgListNode, T>>* v = nullptr>
    T* As() {
        return static_cast<T*>(this);
    }

protected:
    friend class AgList;
    friend class AgTimer;
    AgListNode* prev_{};
    AgListNode* next_{};
};

class AgList final {
public:
    constexpr AgList() = default;

    void PushBack(AgListNode& item);
    void PushFront(AgListNode& item);
    void Remove(AgListNode& item);
    NullablePtr<AgListNode> PopBack();
    NullablePtr<AgListNode> PopFront();
    ag_bool IsEmpty() const;
    AgList Cut(AgListNode& item);
    void Append(AgList& other);

    AgListNode* GetHead() const { return head_; }
    AgListNode* GetTail() const { return tail_; }

    ag_bool Contain(AgListNode& item) const;

    // ---------------------------------------- iterator ----------------------------------------
    struct Iterator {
        AgListNode* node_{};
        explicit Iterator(AgListNode* node) : node_(node) {}
        constexpr bool operator!=(const Iterator& other) const { return node_ != other.node_; }
        constexpr Iterator& operator++() { node_ = node_->next_; return *this; }
        constexpr AgListNode& operator*() const { return *node_; }
    };

    Iterator begin() const { return Iterator{head_}; }
    Iterator end() const { return Iterator{nullptr}; }

    template<class F>
    void ForEach(F&& f) {
        AgListNode* node = head_;
        while (nullptr != node) {
            f(node);
            node = node->next_;
        }
    }

    template<class F>
    void RemoveAll(F&& deinit) {
        AgListNode* node = head_;
        while (nullptr != node) {
            AgListNode* next = node->next_;
            deinit(node);
            node = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
    }

private:
    AgListNode* head_{};
    AgListNode* tail_{};
};

}
