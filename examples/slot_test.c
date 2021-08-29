#include "slot_test.h"
#include "signal_test.h"

#include <stdio.h>

static void _slot(int index, void *sender, char *content)
{
    printf("[slot%d] ", index);
    printf("sender:");
    if (IS_SENDER(signal1))
        printf("signal1");
    else if (IS_SENDER(signal2))
        printf("signal2");
    else
        printf("unknown");
    printf(" content:%s\n", content);
}

SLOT_DEF(slot1, char *str)
{
    _slot(1, sender, str);
}

SLOT_DEF(slot2, char *str)
{
    _slot(2, sender, str);
}