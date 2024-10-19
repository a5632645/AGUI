#include "agui/ag_button.h"

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

void AgButton_Init(AgButton* button) {
    button->state = eAgButtonState_Idel;
#ifdef AGUI_BUTTON_TIME_NEED
    button->press_time = 0;
#endif
#if AGUI_BUTTON_SHORT_LONG_TRIGGER == 1 && AGUI_BUTTON_CONST_SHORT_TIME == 0
    button->short_time = 0;
#endif
#if AGUI_BUTTON_LONG_PRESS_CLICK == 1
    button->long_press_time_left = 0;
#if AGUI_BUTTON_CONST_LONG_PRESS_INTERVAL == 0
    button->long_press_interval = 0;
#endif
#if AGUI_BUTTON_CONST_LONG_PRESS_CLICK_ENTER_TIME == 0
    button->enter_long_press_click_time = 0;
#endif
#endif
#if AGUI_BUTTON_EVENT_FILTER == 1
    button->event_filter.click = 0;
    button->event_filter.release = 0;
    button->event_filter.long_trigger = 0;
    button->event_filter.short_trigger = 0;
    button->event_filter.long_press_click = 0;
#endif
    button->event = NULL;
}

void AgButton_Tick(AgButton* button, ag_bool press, ag_uint16 escape) {
    switch (button->state) { /* 状态转移 */
    case eAgButtonState_Idel:
        if (press) {
            button->state = eAgButtonState_Click;
        }
        break;
    case eAgButtonState_Click:
        if (press) {
            button->state = eAgButtonState_Press;
        }
        else {
            button->state = eAgButtonState_Release;
        }
        break;
    case eAgButtonState_Press:
        if (!press) {
            button->state = eAgButtonState_Release;
        }
        break;
    case eAgButtonState_Release:
        if (press) {
            button->state = eAgButtonState_Click;
        }
        else {
            button->state = eAgButtonState_Idel;
        }
        break;
    default:
        break;
    }

    switch (button->state) { /* 状态处理 */
    case eAgButtonState_Click:
#ifdef AGUI_BUTTON_TIME_NEED
        button->press_time = 0;
#endif
#if AGUI_BUTTON_LONG_PRESS_CLICK == 1
        button->long_press_time_left = BTN_LONG_PRESS_CLICK_ENTER_TIME(button) + BTN_LONG_PRESS_INTERVAL(button);
#endif
        BTN_EVENT_IF(button->event_filter.click) {
            button->event(button, eAgButtonEvent_Click);
        }
        break;
    case eAgButtonState_Press:
#ifdef AGUI_BUTTON_TIME_NEED
        button->press_time += escape;
#endif
#if AGUI_BUTTON_LONG_PRESS_CLICK == 1
        BTN_EVENT_IF (button->event_filter.long_press_click) {
            button->long_press_time_left -= escape;
            if (button->long_press_time_left <= 0) {
                button->long_press_time_left = BTN_LONG_PRESS_INTERVAL(button);
                button->event(button, eAgButtonEvent_LongPressClick);
            }
        }
#endif
        break;
    case eAgButtonState_Release:
        BTN_EVENT_IF (button->event_filter.release) {
            button->event(button, eAgButtonEvent_Release);
        }
#if AGUI_BUTTON_SHORT_LONG_TRIGGER == 1
        BTN_EVENT_IF (button->event_filter.long_trigger) {
            if (button->press_time > BTN_SHORT_TIME(button)) {
                button->event(button, eAgButtonEvent_LongTrigger);
            }
        }
        BTN_EVENT_IF (button->event_filter.short_trigger) {
            if (button->press_time < BTN_SHORT_TIME(button)) {
                button->event(button, eAgButtonEvent_ShortTrigger);
            }
        }
#endif
        break;
    }
}