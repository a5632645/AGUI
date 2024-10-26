#pragma once
#include "agui/ag_types.hpp"
#include "agui/ag_rect.hpp"
#include "agui/ag_draw.hpp"
#include "ag_impl/ag_cfg.hpp"

namespace agui {

class AgPainter {
public:
    virtual ~AgPainter() = default;
    // ---------------------------------------- interface ----------------------------------------
    virtual NullablePtr<void> GetBackend() = 0;                   /* 获取绘制后端，如u8g2,adfurit等 */
    virtual void CallDraw(const AgDraw& arg) = 0;   /* 绘制东西 */
    virtual void BeginFrame() = 0;                    /* 开始新的绘制 */
    virtual void EndFrame() = 0;                      /* 结束绘制 */

    const AgRect& GetDrawAera() const;
    AgRect GetLocalDrawAera() const;

    void PrepareDraw(const AgRect& global_aera);
    void SaveState();
    void RestoreState();
    AgRect& ModifyCurrDrawAera();
protected:
    friend class AgObj;
    // ---------------------------------------- internal ----------------------------------------
    AgRect draw_aera_{};       /* 临时变量，代表全局绘制区域 */
private:
    ag_bool always_redraw_{};  /* 标记永远绘制。当某个节点标记重绘时，该位被标记 */
    ag_int16 depth_{};         /* 递归迭代位置 */
    struct PainterData {  /* 迭代保存数据 */
        AgRect draw_aera{};
        ag_bool always_redraw{};
    } data_[AGUI_PAINTER_MAX_STEPS]{};
};

}
