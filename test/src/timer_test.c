#include "agui/ag_timer.h"
#include "agui/ag_types.h"
#include <stdio.h>
#include <time.h>

static void Timer1(AgTimer* timer) {
    printf("Timer1\n");
}

static void Timer2(AgTimer* timer) {
    printf("Timer2\n");
}

static void Timer3(AgTimer* timer) {
    printf("Timer3\n");
    static int count = 0;
    ++count;
    if (count == 3) {
        timer->repeat = ag_false;
    }
}

static void Timer4(AgTimer* timer) {
    printf("Timer4\n");
    static int count = 0;
    ++count;
    if (count == 3) {
        AgTimer_Remove(timer);
    }
}

int main(void) {
    AgTimer group;
    AgTimer_Init(&group);
    group.timeout = Timer1;
    group.period = 1;
    group.repeat = ag_true;

    AgTimer timer2;
    AgTimer_Init(&timer2);
    timer2.timeout = Timer2;
    timer2.period = 1;
    timer2.repeat = ag_false;
    AgTimer_Add(&group, &timer2);

    AgTimer timer3;
    AgTimer_Init(&timer3);
    timer3.timeout = Timer3;
    timer3.period = 1;
    timer3.repeat = ag_true;
    AgTimer_Add(&group, &timer3);

    AgTimer timer4;
    AgTimer_Init(&timer4);
    timer4.timeout = Timer4;
    timer4.period = 1;
    timer4.repeat = ag_true;
    AgTimer_Add(&group, &timer4);

    time_t begin = time(NULL);
    for (;;) {
        time_t end = time(NULL);
        while (end - begin < 1) {
            end = time(NULL);
        }
        ag_uint32 escape = end - begin;
        begin = end;
        AgTimer_Tick(&group, escape);
    }
}