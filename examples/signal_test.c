#include "signal_test.h"

SIGNAL_DEF(signal1, char *str)
{
    SIGNAL_SEND(signal1, str);
}

SIGNAL_DEF(signal2, char *str)
{
    SIGNAL_SEND(signal2, str);
}