#pragma once
#include "agui/ag_list.hpp"
#include "agui/ag_types.hpp"
#include "agui/ag_rect.hpp"
#include "agui/ag_painter.hpp"
#include "agui/ag_event.hpp"
#include "agui/ag_align.hpp"

namespace agui {

/**
 * @brief 节点，默认填充黑色
 */
class AgObj : public AgListNode {
public:
    ~AgObj() override;
    virtual void Draw(AgPainter& painter);
    virtual void Layout() {}
    virtual void Event(AgEvent& event) {}

    NullablePtr<AgObj> FirstChild();
    NullablePtr<AgObj> NextSibling();
    NullablePtr<AgObj> PrevSibling();
    NullablePtr<AgObj> LastChild();

    AgObj(AgObj* parent = nullptr);

    void AddChild(AgObj& child);
    void AddChildAtBack(AgObj& child);
    void AddChildFromArray(AgObj* array, ag_uint32 count);
    void RemoveChild(AgObj& child);
    void RemoveAllChild();
    void LeaveParent();
    void DrawObj(AgPainter& painter);
    void DrawObjInObj(AgPainter& painter);
    ag_bool HitTest(ag_int16 x, ag_int16 y);
    NullablePtr<AgObj> HitObj(ag_int16 x, ag_int16 y);

    void SetVisible(ag_bool visiable);
    void ReDraw();
    void RedrawRoot();
    void DoLayout();
    ag_bool NeedDraw() const;
    
    void SetBound(const AgRect& bound);
    void SetBounds(ag_int16 x, ag_int16 y, ag_int16 w, ag_int16 h);
    void SetPos(ag_int16 x, ag_int16 y);
    void SetSize(ag_int16 w, ag_int16 h);
    AgRect GetLocalBound() const;
    const AgRect& GetBound() const { return bound_; }
    
    void BringToFront();
    void SendToBack();

    void CalcBound();
    void SendEvent(AgEvent& event);

    void FastLayout(AgAlignEnum align);
protected:
    void _CalcSingleBound();
    void _ReDraw();
    void _PopUpRedraw(const AgRect& rect);

    AgObj* parent_{}; /* 父节点 */
    AgList childern_;        /* 子节点，注意不会释放内存 */
    struct {
        ag_bool redraw : 1;     /* 需要重新绘制 */
        ag_bool invalid : 1;    /* 无效，但不代表需要重新绘制 */
        ag_bool visiable : 1;   /* 可见性 */
        ag_bool transpant : 1;  /* 是否是半透明 */
    } flags_;
    AgRect bound_; /* 在父节点空间的位置 */
};

}
