#include "agui/ag_rect.h"

void AgRect_Zero(AgRect* rect) {
    rect->x = 0;
    rect->y = 0;
    rect->w = 0;
    rect->h = 0;
}

ag_bool AgRect_Equal(const AgRect* rect1, const AgRect* rect2) {
    if (rect1->x != rect2->x) {
        return ag_false;
    }
    if (rect1->y != rect2->y) {
        return ag_false;
    }
    if (rect1->w != rect2->w) {
        return ag_false;
    }
    if (rect1->h != rect2->h) {
        return ag_false;
    }
    return ag_true;
}