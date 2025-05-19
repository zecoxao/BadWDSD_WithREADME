#include "Include.h"

void Hold_Init()
{
    if (!Gpio_GetOnce(HOLD_PIN_ID))
        dead();
}