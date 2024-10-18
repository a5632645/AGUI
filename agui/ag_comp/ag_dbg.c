#include "ag_comp/ag_dbg.h"

static AgColor Hue2Rgb(uint8_t hue, uint8_t brightness) {
    uint8_t r = 0, g = 0, b = 0;
    if(hue < 60) {
        r = brightness;
        g = hue * 3 * brightness / 100;
        b = 0;
    } else if(hue < 120) {
        r = brightness - hue * 3 * brightness / 100;
        g = brightness;
        b = 0;
    } else if(hue < 180) {
        r = 0;
        g = brightness;
        b = hue * 3 * brightness / 100;
    } else if(hue < 240) {
        r = 0;
        g = brightness - hue * 3 * brightness / 100;
        b = brightness;
    } else if(hue < 300) {
        r = hue * 3 * brightness / 100;
        g = 0;
        b = brightness;
    } else {
        r = brightness;
        g = 0;
        b = brightness - hue * 3 * brightness / 100;
    }
    return (AgColor){r, g, b, 255};
}

static AgColor AgDbg_RandomColor() {
    static uint8_t last_hue = 0;
    last_hue += 60;
    last_hue %= 360;
    return Hue2Rgb(last_hue, 255);
}

void AgDbg_DrawFrame(AgObj* obj, AgPainter* painter) {
    #if AGUI_DBG_WRIE_FRAME == 1
    AgRectDraw draw = {
        .color = AgDbg_RandomColor()
    };
    AgRectDraw_Init(&draw, painter);
    AgObj_GetLocalBound(obj, &draw.rect);
    painter->call_draw(painter, &draw.draw);
    #endif
}
