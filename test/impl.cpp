#include "ag_impl/ag_impl.hpp"
#include "agui/ag_obj.hpp"
#include "agui/ag_painter.hpp"
#include "agui/ag_color.hpp"
#include "draws.hpp"
#include <cstdlib>

namespace agui::impl {

void AgEmptyObjDraw(AgObj& obj, AgPainter& painter) {
    FillDraw draw;
    draw.color = colors::kBlack;
    draw.rect = obj.GetLocalBound();
    painter.CallDraw(draw);
}

void AgDrawColorFrame(AgPainter& painter, const AgRect& aera, AgColor color) {
    RectDraw draw;
    draw.color = color;
    draw.rect = aera;
    painter.CallDraw(draw);
}


/* 锁 */
void AgImpl_LockInit(ag_lock_t& lock) {
}

void AgImpl_LockDestroy(ag_lock_t& lock) {
}

void AgImpl_Lock(ag_lock_t& lock) {
    lock.lock();
}

void AgImpl_Unlock(ag_lock_t& lock) {
    lock.unlock();
}


/* 内存 */
void* AgImpl_MemAlloc(ag_size_t size) {
    return malloc(size);
}

void AgImpl_MemFree(void* mem) {
    free(mem);
}


}
