/*
 * @file ag_list_view.h
 * @brief agui list view system
*/

#pragma once
#include "ag_comp/deps.hpp"

namespace agui {

// ---------------------------------------- view ----------------------------------------

enum class AgListViewStyle : ag_uint8 {
    eAgListViewStyle_Horizontal = 0,
    eAgListViewStyle_Vertical
};

class AgListView;
class AgListModel {
public:
    virtual ~AgListModel() = default;
    virtual ag_int16 Count() = 0;
    virtual ag_int16 Length(AgListViewStyle style, ag_int16 idx) = 0;
    virtual void Draw(AgPainter& painter, ag_int16 idx, ag_bool selected) = 0;
    virtual void Event(AgListView& lv, AgEvent& event, ag_int16 idx) = 0;
};

class AgListViewDelegate {
public:
    virtual ~AgListViewDelegate() = default;
    virtual ag_int16 ScrollBarWidth(AgListViewStyle) = 0;
    virtual void ScrollBar(AgPainter& painter, AgListViewStyle style, ag_int16 pixel_start, ag_int16 length) = 0;
    virtual void Background(AgPainter& painter, const AgRect& draw_aera) = 0;
};

class AgListView : public AgObj {
public:
    AgListView(AgObj* parent = nullptr);

    void SetModel(AgListModel* model);
    void SetDelegate(AgListViewDelegate* delegate);
    void NotifyItemChange(ag_int16 idx);
    void Update();
    void SetBeginIdx(ag_int16 idx);
    void Scroll(ag_int16 shift);
    void SetSelectedIdx(ag_int16 idx);
    ag_int16 GetSelectedIdx() const { return select_idx_; }
    void SetStyle(AgListViewStyle style);

    void Draw(AgPainter& painter) override;
    void Layout() override;
    void Event(AgEvent& event) override;
private:
    void __Draw_Vertical(AgPainter& painter);
    void __Draw_Horizontal(AgPainter& painter);
    ag_int16 _GetHowManyItems(ag_int16 idx, ag_bool is_down);
    void _SetEndIdx(ag_int16 idx);
    void _ScrollUp(ag_int16 shift);
    void _ScrollDown(ag_int16 shift);
    void _NullModelUpdate();

    inline static class EmptyDelegate : public AgListViewDelegate {
    public:
        ag_int16 ScrollBarWidth(AgListViewStyle style) override { return 0; }
        void ScrollBar(AgPainter& painter, AgListViewStyle style, ag_int16 pixel_start, ag_int16 length) override {}
        void Background(AgPainter& painter, const AgRect& draw_aera) override;
    } s_empty_delegate_;

    AgListViewDelegate* delegate_{ &s_empty_delegate_ };
    AgListModel* model_{ nullptr };

    ag_int16 begin_idx_;         /* 最顶部的obj的idx */
    ag_int16 display_count_;     /* 显示的数量 */
    ag_int16 select_idx_;        /* 选中的obj的idx */
    AgListViewStyle style_{ AgListViewStyle::eAgListViewStyle_Vertical };     /* 横向或者纵向 */
};

}
