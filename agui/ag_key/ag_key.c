#include "ag_key/ag_key.h"

// ---------------------------------------- private ----------------------------------------
static void _AddToSameLayer(AgObj* obj, AgObj* highlight) {
    AgObj_LeaveParent(highlight);
    if (NULL == obj->parent) {
        AgObj_AddChildAtBack(obj, highlight);
    }
    else {
        AgObj_AddChildAtBack(obj->parent, highlight);
    }
    AgObj_SetBound(highlight, &obj->bound);
}

// ---------------------------------------- impl ----------------------------------------
static ag_bool _Filter(AgObj*) {
    return ag_true;
}

static ag_bool _ActionFilter(AgObj*, AgKeySwitActionEnum) {
    return ag_true;
}

static void _ObjSelected(AgObj*) {}

// ---------------------------------------- public ----------------------------------------
/**
 * @brief 初始化
 * @param ks 
 * @param root 
 */
void AgKeySwitcher_Init(AgKeySwitcher* ks, AgObj* root, AgObj* highlight) {
    ks->root = root;
    ks->current = ks->root;
    ks->highlight = highlight;
    AgObj_AddChildAtBack(ks->root, ks->highlight);
    AgObj_SetBound(ks->highlight, &ks->current->bound);

    ks->filter = _Filter;
    ks->action_filter = _ActionFilter;
    ks->obj_selected = _ObjSelected;
}

/**
 * @brief 设置当前物体
 * @param ks 
 * @param obj 
 */
void AgKeySwitcher_SetCurrent(AgKeySwitcher* ks, AgObj* obj) {
    ks->current = obj;
    _AddToSameLayer(obj, ks->highlight);
    ks->obj_selected(obj);
}

/**
 * @brief 深入到当前节点的下一层
 * @param ks 
 */
void AgKeySwitcher_GoDown(AgKeySwitcher* ks) {
    if (ag_false == ks->action_filter(ks->current, eAgKeyAction_GoDown)) {
        return;
    }
    if (NULL == ks->current) {
        return;
    }
    if (ag_true == AgList_IsEmpty(&ks->current->childern)) {
        return;
    }
    AgListNode* node = ks->current->childern.head;
    AgObj* obj = AGUI_CONTAINER_OF(AgObj, node, node);
    while (NULL != node
        && obj == ks->highlight
        && ag_true == obj->flags.visiable
        && ag_true == ks->filter(obj)) {
        node = node->next;
        obj = AGUI_CONTAINER_OF(AgObj, node, node);
    }
    if (NULL != node) {
        ks->current = obj;
        _AddToSameLayer(ks->current, ks->highlight);
    }
    else {
        AgKeySwitcher_GoRoot(ks);
    }
    ks->obj_selected(ks->current);
}

/**
 * @brief 回到当前节点的上一层
 * @param ks 
 */
void AgKeySwitcher_GoUp(AgKeySwitcher* ks) {
    if (ag_false == ks->action_filter(ks->current, eAgKeyAction_GoUp)) {
        return;
    }
    if (NULL == ks->current) {
        return;
    }
    if (NULL == ks->current->parent) {
        return;
    }
    ks->current = ks->current->parent;
    _AddToSameLayer(ks->current, ks->highlight);
    ks->obj_selected(ks->current);
}

/**
 * @brief 移动到同级节点下一个
 * @param ks 
 */
void AgKeySwitcher_GoNext(AgKeySwitcher* ks) {
    if (ag_false == ks->action_filter(ks->current, eAgKeyAction_GoNext)) {
        return;
    }
    do {
        if (NULL == ks->current->node.next) {
            if (NULL != ks->current->parent) {
                ks->current = AGUI_CONTAINER_OF(AgObj, node, ks->current->parent->childern.head);
            }
        }
        else {
            ks->current = AGUI_CONTAINER_OF(AgObj, node, ks->current->node.next);
        }
    } while (ks->current == ks->highlight 
            || (NULL != ks->current && ks->current->flags.visiable == ag_false)
            || ks->filter(ks->current) == ag_false);
    _AddToSameLayer(ks->current, ks->highlight);
    ks->obj_selected(ks->current);
}

/**
 * @brief 移动到同级节点上一个
 * @param ks 
 */
void AgKeySwitcher_GoPrev(AgKeySwitcher* ks) {
    if (ag_false == ks->action_filter(ks->current, eAgKeyAction_GoPrev)) {
        return;
    }
    do {
        if (NULL == ks->current->node.prev) {
            if (NULL != ks->current->parent) {
                ks->current = AGUI_CONTAINER_OF(AgObj, node, ks->current->parent->childern.tail);
            }
        }
        else {
            ks->current = AGUI_CONTAINER_OF(AgObj, node, ks->current->node.prev);
        }
    } while (ks->current == ks->highlight
            || (NULL != ks->current && ks->current->flags.visiable == ag_false)
            || ks->filter(ks->current) == ag_false);
    _AddToSameLayer(ks->current, ks->highlight);
    ks->obj_selected(ks->current);
}

/**
 * @brief 回到跟节点
 * @param ks 
 */
void AgKeySwitcher_GoRoot(AgKeySwitcher* ks) {
    if (ag_false == ks->action_filter(ks->current, eAgKeyAction_GoRoot)) {
        return;
    }
    ks->current = ks->root;
    _AddToSameLayer(ks->current, ks->highlight);
    ks->obj_selected(ks->current);
}