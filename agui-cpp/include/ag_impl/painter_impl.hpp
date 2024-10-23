/*
 * \file painter_impl.h
 * \brief 在这里实现AgPainter接口，需要自己适配，可能要移出去
*/

#pragma once
#include "agui/ag_painter.hpp"
#include "raylib.h"

namespace agui::impl {

class PainterImpl : public AgPainter {
public:
    PainterImpl();
    ~PainterImpl() override;

    NullablePtr<void> GetBackend() const;                   /* 获取绘制后端，如u8g2,adfurit等 */
    void CallDraw(const AgDraw& arg);   /* 绘制东西 */
    void BeginFrame();                    /* 开始新的绘制 */
    void EndFrame();                      /* 结束绘制 */
private:
    void DrawText(const AgTextDraw& arg);

    RenderTexture2D texture_;
};

}
