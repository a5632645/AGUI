#pragma once
#include "agui/ag_event.h"

enum AgMainEvent {
    eAgEvent_Button = 0,  
};

typedef struct {
    AgEvent event;
    ag_bool pressed;
    ag_uint16 key;
} ButtonEvent;
void ButtonEvent_Init(ButtonEvent* event);