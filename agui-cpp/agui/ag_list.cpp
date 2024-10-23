#include "agui/ag_list.hpp"
#include "agui/ag_types.hpp"

namespace agui {

void AgList::PushBack(AgListNode& item) {
    item.next_ = tail_ ? tail_->next_ : NULL;
    item.prev_ = tail_;

    if (tail_) {
        tail_->next_ = &item;
    } else {
        head_ = &item;
    }

    tail_ = &item;
}

void AgList::PushFront(AgListNode& item) {
    item.next_ = head_;
    item.prev_ = head_ ? head_->prev_ : NULL;

    if (head_) {
        head_->prev_ = &item;
    } else {
        tail_ = &item;
    }

    head_ = &item;
}

void AgList::Remove(AgListNode& item) {
    if (item.prev_) {
        item.prev_->next_ = item.next_;
    } else {
        head_ = item.next_;
    }

    if (item.next_) {
        item.next_->prev_ = item.prev_;
    } else {
        tail_ = item.prev_;
    }

    item.next_ = NULL;
    item.prev_ = NULL;
}

NullablePtr<AgListNode> AgList::PopBack() {
    if (!tail_) {
        return NULL;
    }

    AgListNode* tail = tail_;
    Remove(*tail);
    return tail;
}

NullablePtr<AgListNode> AgList::PopFront() {
    if (head_) {
        return NULL;
    }

    AgListNode* head = head_;
    Remove(*head);
    return head;
}

ag_bool AgList::IsEmpty() const {
    return NULL == head_ && NULL == tail_;
}

AgList AgList::Cut(AgListNode& node) {
    AgList ret;
    /* 连接 */
    ret.tail_ = tail_;
    ret.head_ = &node;
    /* 检查 */
    if (&node == head_) {
        head_ = NULL;
        tail_ = NULL;
    }
    /* 断开连接 */
    tail_ = node.prev_;
    if (NULL != node.prev_) {
        node.prev_->next_ = NULL;
    }
    node.prev_ = NULL;
    return ret;
}

void AgList::Append(AgList& other) {
    if (other.IsEmpty()) {
        return;
    }
    
    if (IsEmpty()) {
        head_ = other.head_;
        tail_ = other.tail_;
        other.head_ = NULL;
        other.tail_ = NULL;
    }
    else {
        other.head_->prev_ = tail_;
        tail_->next_ = other.head_;
        tail_ = other.tail_;
        other.head_ = NULL;
        other.tail_ = NULL;
    }
}

}