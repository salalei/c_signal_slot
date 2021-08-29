#include "../signal_slot.h"
#include <stdio.h>

#include "signal_test.h"
#include "slot_test.h"

int main(int argc, char **argv)
{
    CONNECT(signal1, slot1);
    CONNECT(signal1, slot2);
    CONNECT(signal2, slot1);
    CONNECT(signal2, slot2);
    CONNECT(signal1, signal2);

    EMIT(signal1, "hello");
    EMIT(signal2, "world");

    return 0;
}