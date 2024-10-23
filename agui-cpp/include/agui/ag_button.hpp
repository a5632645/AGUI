#pragma once
#include "agui/ag_types.hpp"

namespace agui {

// ---------------------------------------- code clip ----------------------------------------
#define AGUI_BUTTON_SHORT_LONG_TRIGGER 1
#define AGUI_BUTTON_LONG_PRESS_CLICK 1
#define AGUI_BUTTON_EVENT_FILTER 1

// ---------------------------------------- options ----------------------------------------
/* 长短按区分时间，0为使用变量 */
#define AGUI_BUTTON_CONST_SHORT_TIME 1
/* 长按情况下触发事件的间隔，0为使用变量 */
#define AGUI_BUTTON_CONST_LONG_PRESS_INTERVAL 1
/* 进入长按情况下触发事件的预延迟时间，0为使用变量 */
#define AGUI_BUTTON_CONST_LONG_PRESS_CLICK_ENTER_TIME 1

// ---------------------------------------- private marcos ----------------------------------------
#if AGUI_BUTTON_SHORT_LONG_TRIGGER == 1 || AGUI_BUTTON_LONG_PRESS_CLICK == 1
#define AGUI_BUTTON_TIME_NEED
#endif

enum class AgButtonStateEnum : ag_uint8 {
    eAgButtonState_Idel = 0,
    eAgButtonState_Click = 1,
    eAgButtonState_Press = 2,
    eAgButtonState_Release = 3
};

enum class AgButtonEvent : ag_uint8 {
    eAgButtonEvent_Click = 0,           /* 按键点击 */
    eAgButtonEvent_Release = 1,         /* 按键弹起 */
#if AGUI_BUTTON_SHORT_LONG_TRIGGER == 1
    eAgButtonEvent_LongTrigger = 2,     /* 按键长按 */
    eAgButtonEvent_ShortTrigger = 3,    /* 按键短按 */
#endif
#if AGUI_BUTTON_LONG_PRESS_CLICK == 1
    eAgButtonEvent_LongPressClick = 4,  /* 按键一直按下 */
#endif
};

class AgButton final {
public:
    void SetListener(NullablePtr<void(*)(AgButton&, AgButtonEvent)> callback);
    void Tick(ag_bool press, ag_uint16 escape);

private:
    NullablePtr<void(*)(AgButton&, AgButtonEvent)> callback_;

    AgButtonStateEnum state{};
#ifdef AGUI_BUTTON_TIME_NEED
    ag_uint16 press_time{};
#endif
#if AGUI_BUTTON_SHORT_LONG_TRIGGER == 1 && AGUI_BUTTON_CONST_SHORT_TIME == 0
    ag_uint16 short_time{};
#endif
#if AGUI_BUTTON_LONG_PRESS_CLICK == 1
#if AGUI_BUTTON_CONST_LONG_PRESS_INTERVAL == 0
    ag_uint16 long_press_interval{};
#endif
    ag_int16 long_press_time_left{};
#if AGUI_BUTTON_CONST_LONG_PRESS_CLICK_ENTER_TIME == 0
    ag_uint16 enter_long_press_click_time{};
#endif
#endif
#if AGUI_BUTTON_EVENT_FILTER == 1
    struct {
        ag_bool click : 1;
        ag_bool release : 1;
        ag_bool long_trigger : 1;
        ag_bool short_trigger : 1;
        ag_bool long_press_click : 1;
    } event_filter{};
#endif
};

}
