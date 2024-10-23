#include "ag_comp/ag_stack_layout.hpp"

// ---------------------------------------- declare ----------------------------------------
static void _Layout(AgObj* obj);
static AgObj* _ChangeObj(AgStackLayout* sl, AgObj* obj);

// ---------------------------------------- private ----------------------------------------
static AgObj* _ChangeObj(AgStackLayout* sl, AgObj* obj) {
    AgObj* curr = AgStackLayout_Current(sl);
    if (NULL != curr) {
        AgObj_LeaveParent(curr);
    }
    AgObj_AddChild(&sl->obj, obj);
    _Layout(&sl->obj);
    AgObj_Redraw(&sl->obj);
    return curr;
}

// ---------------------------------------- impl ----------------------------------------
static void _Layout(AgObj* obj) {
    AgStackLayout* sl = AGUI_CONTAINER_OF(AgStackLayout, obj, obj);
    AgObj* curr = AgStackLayout_Current(sl);
    if (NULL != curr) {
        AgRect local;
        AgObj_GetLocalBound(obj, &local);
        AgObj_SetBound(curr, &local);
    }
}

// ---------------------------------------- public ----------------------------------------
void AgStackLayout_Init(AgStackLayout* sl) {
    AgObj_Init(&sl->obj);
    AgList_Init(&sl->stack);

    sl->obj.vfunc.layout = _Layout;
    sl->obj.obj_type = eAgObjType_StackLayout;
}

void AgStackLayout_Push(AgStackLayout* sl, AgObj* obj) {
    if (NULL == obj || obj == AgStackLayout_Current(sl)) {
        return;
    }
    AgObj* old = _ChangeObj(sl, obj);
    if (NULL != old) {
        AgList_PushBack(&sl->stack, &old->node);
    }
    AgObj_Redraw(&sl->obj);
}

void AgStackLayout_Push2(AgStackLayout* sl, AgObj* obj) {
    if (NULL == obj || obj == AgStackLayout_Current(sl)) {
        return;
    }
    /* 在栈中查找 */
    AgListNode* node = sl->stack.head;
    while (NULL != node) {
        AgObj* o = AGUI_CONTAINER_OF(AgObj, node, node);
        if (o == obj) {
            break;
        }
        node = node->next;
    }
    /* 未找到 */
    if (NULL == node) {
        AgStackLayout_Push(sl, obj);
        return;
    }
    /* 找到，从栈中删除 */
    AgList_Remove(&sl->stack, &obj->node);
    AgStackLayout_Push(sl, obj);
}

void AgStackLayout_Push3(AgStackLayout* sl, AgList* ret, AgObj* obj) {
    if (NULL == obj || obj == AgStackLayout_Current(sl)) {
        AgList_Init(ret);
        return;
    }
    /* 在栈中查找 */
    AgListNode* node = sl->stack.head;
    while (NULL != node) {
        AgObj* o = AGUI_CONTAINER_OF(AgObj, node, node);
        if (o == obj) {
            break;
        }
        node = node->next;
    }
    /* 未找到 */
    if (NULL == node) {
        AgStackLayout_Push(sl, obj);
        AgList_Init(ret);
        return;
    }
    /* 找到，从栈中删除 */
    AgList_Cut(&sl->stack, ret, &obj->node);
    AgList_Popfront(ret);
    AgObj* old = _ChangeObj(sl, obj);
    if (NULL != old) {
        AgList_PushBack(ret, &old->node);
    }
}

AgObj* AgStackLayout_Pop(AgStackLayout* sl) {
    AgObj* ret = NULL;
    AgListNode* top = AgList_Popback(&sl->stack);
    if (NULL != top) {
        AgObj* obj = AGUI_CONTAINER_OF(AgObj, node, top);
        ret = _ChangeObj(sl, obj);
    }
    return ret;
}

AgObj* AgStackLayout_Current(AgStackLayout* sl) {
    AgListNode* curr = sl->obj.childern.head;
    AgObj* obj = AGUI_CONTAINER_OF(AgObj, node, curr);
    return obj;
}