#include "agui/layout/ag_im_layout.h"

void AgImLayout_Layout(AgObj* obj, AgAlignEnum align) {
    if (NULL == obj->parent) {
        return;
    }

    AgAlignEnum x_align = align & 0b11;
    const AgRect* parent_bound = &obj->parent->bound;
    const AgRect* bound = &obj->bound;
    switch (x_align) {
    case eAgAlign_XCenter:
        AgObj_SetPos(obj, parent_bound->w / 2 - bound->w / 2, bound->y);
        break;
    case eAgAlign_XLeft:
        AgObj_SetPos(obj, 0, bound->y);
        break;
    case eAgAlign_XRight:
        AgObj_SetPos(obj, parent_bound->w - bound->w, bound->y);
        break;
    default:
        break;
    }

    AgAlignEnum y_align = align & 0b1100;
    switch (y_align) {
    case eAgAlign_YCenter:
        AgObj_SetPos(obj, bound->x, parent_bound->h / 2 - bound->h / 2);
        break;
    case eAgAlign_YTop:
        AgObj_SetPos(obj, bound->x, 0);
        break;
    case eAgAlign_YBottom:
        AgObj_SetPos(obj, bound->x, parent_bound->h - bound->h);
        break;
    default:
        break;
    }
}