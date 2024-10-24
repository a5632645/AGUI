#include "agui/ag_button.hpp"

namespace agui {

// ---------------------------------------- private ----------------------------------------
#if AGUI_BUTTON_EVENT_FILTER == 1
#define BTN_EVENT_IF(IF) if (IF)
#else
#define BTN_EVENT_IF(IF)
#endif

#if AGUI_BUTTON_CONST_SHORT_TIME == 0
#define BTN_SHORT_TIME(BTN) (BTN->short_time)
#else
#define BTN_SHORT_TIME(BTN) AGUI_BUTTON_CONST_SHORT_TIME
#endif
#if AGUI_BUTTON_CONST_LONG_PRESS_INTERVAL == 0
#define BTN_LONG_PRESS_INTERVAL(BTN) (BTN->long_press_interval)
#else
#define BTN_LONG_PRESS_INTERVAL(BTN) AGUI_BUTTON_CONST_LONG_PRESS_INTERVAL
#endif
#if AGUI_BUTTON_CONST_LONG_PRESS_CLICK_ENTER_TIME == 0
#define BTN_LONG_PRESS_CLICK_ENTER_TIME(BTN) (BTN->enter_long_press_click_time)
#else
#define BTN_LONG_PRESS_CLICK_ENTER_TIME(BTN) AGUI_BUTTON_CONST_LONG_PRESS_CLICK_ENTER_TIME
#endif

// ---------------------------------------- public ----------------------------------------
void AgButton::SetListener(NullablePtr<void(*)(AgButton&, AgButtonEvent)> callback) {
    callback_ = callback;
}

void AgButton::Tick(ag_bool press, ag_uint16 escape) {
    switch (state) { /* 状态转移 */
    case AgButtonStateEnum::eAgButtonState_Idel:
        if (press) {
            state = AgButtonStateEnum::eAgButtonState_Click;
        }
        break;
    case AgButtonStateEnum::eAgButtonState_Click:
        if (press) {
            state = AgButtonStateEnum::eAgButtonState_Press;
        }
        else {
            state = AgButtonStateEnum::eAgButtonState_Release;
        }
        break;
    case AgButtonStateEnum::eAgButtonState_Press:
        if (!press) {
            state = AgButtonStateEnum::eAgButtonState_Release;
        }
        break;
    case AgButtonStateEnum::eAgButtonState_Release:
        if (press) {
            state = AgButtonStateEnum::eAgButtonState_Click;
        }
        else {
            state = AgButtonStateEnum::eAgButtonState_Idel;
        }
        break;
    default:
        break;
    }

    switch (state) { /* 状态处理 */
    case AgButtonStateEnum::eAgButtonState_Click:
#ifdef AGUI_BUTTON_TIME_NEED
        press_time = 0;
#endif
#if AGUI_BUTTON_LONG_PRESS_CLICK == 1
        long_press_time_left = BTN_LONG_PRESS_CLICK_ENTER_TIME(button) + BTN_LONG_PRESS_INTERVAL(button);
#endif
        BTN_EVENT_IF(event_filter.click) {
            callback_.CallIf([this](auto* fn){fn(*this, AgButtonEvent::eAgButtonEvent_Click);});
        }
        break;
    case AgButtonStateEnum::eAgButtonState_Press:
#ifdef AGUI_BUTTON_TIME_NEED
        press_time += escape;
#endif
#if AGUI_BUTTON_LONG_PRESS_CLICK == 1
        BTN_EVENT_IF (event_filter.long_press_click) {
            long_press_time_left -= escape;
            if (long_press_time_left <= 0) {
                long_press_time_left = BTN_LONG_PRESS_INTERVAL(button);
                callback_.CallIf([this](auto* fn){fn(*this, AgButtonEvent::eAgButtonEvent_LongPressClick);});
            }
        }
#endif
        break;
    case AgButtonStateEnum::eAgButtonState_Release:
        BTN_EVENT_IF (event_filter.release) {
            callback_.CallIf([this](auto* fn){fn(*this, AgButtonEvent::eAgButtonEvent_Release);});
        }
#if AGUI_BUTTON_SHORT_LONG_TRIGGER == 1
        BTN_EVENT_IF (event_filter.long_trigger) {
            if (press_time > BTN_SHORT_TIME(button)) {
                callback_.CallIf([this](auto* fn){fn(*this, AgButtonEvent::eAgButtonEvent_LongTrigger);});
            }
        }
        BTN_EVENT_IF (event_filter.short_trigger) {
            if (press_time < BTN_SHORT_TIME(button)) {
                callback_.CallIf([this](auto* fn){fn(*this, AgButtonEvent::eAgButtonEvent_ShortTrigger);});
            }
        }
#endif
        break;
    default:
        break;
    }
}

}
