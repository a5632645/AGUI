#include "ag_impl/event_impl.h"

void ButtonEvent_Init(ButtonEvent* event) {
    event->event.type = eAgEvent_Button;
}