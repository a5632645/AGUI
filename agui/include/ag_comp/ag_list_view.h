/*
 * @file ag_list_view.h
 * @brief agui list view system
*/

#pragma once
#include "ag_comp/deps.h"

// ---------------------------------------- declare ----------------------------------------
struct __AgListModel;

// ---------------------------------------- view ----------------------------------------

typedef struct {
    /* interface */
    AgObj obj;
    /* data */
    struct __AgListModel* model; /* model */
    ag_int16 begin_idx;         /* 最顶部的obj的idx */
    ag_int16 display_count;     /* 显示的数量 */
    ag_int16 select_idx;        /* 选中的obj的idx */
} AgListView;

/**
 * @brief 初始化
 * @param lv 不能为NULL
 * @param parent 可以为NULL
 * @param model 可以为NULL
 */
void AgListView_Init(AgListView* lv, AgObj* parent, struct __AgListModel* model);

/**
 * @brief 设置model
 * @param lv 不能为NULL
 * @param model 可以为NULL
 */
void AgListView_SetModel(AgListView* lv, struct __AgListModel* model);

/**
 * @brief 某个item改变了
 * @param lv 不能为NULL
 * @param idx 任意数
 */
void AgListView_ItemChange(AgListView* lv, ag_int16 idx);

/**
 * @brief 刷新整个list，将触发绘制
 * @param lv 不能为NULL
 */
void AgListView_Update(AgListView* lv);

/**
 * @brief 设置开始的idx
 * @param lv 不能为NULL
 * @param idx 任意数，无效会被clamp到0~model.count
 */
void AgListView_SetBeginIdx(AgListView* lv, ag_int16 idx);

/**
 * @brief 往上或者往下移动
 * @param lv 不能为NULL
 * @param shift 任意数
 */
void AgListView_Scroll(AgListView* lv, ag_int16 shift);

/**
 * @brief 设置选中的idx
 * @param lv 不能为NULL
 * @param idx 任意数
 */
void AgListView_SetSelectIdx(AgListView* lv, ag_int16 idx);

// ---------------------------------------- model ----------------------------------------

typedef struct __AgListModel {
    /* 有多少 */
    ag_int16(*count)();
    /* 有多高 */
    ag_int16(*height)(ag_int16 idx);
    /* 怎么绘制 */
    void(*draw)(AgPainter* painter, ag_int16 idx, ag_bool selected);
    /* 发生了什么 */
    void(*event)(AgListView* lv, AgEvent* event, ag_int16 idx);
} AgListModel;