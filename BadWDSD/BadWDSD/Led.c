#include "Include.h"

volatile bool ledIsInited = false;
volatile struct LedContext_s ledContext;

void Led_SetStatus(uint32_t status)
{
    ledContext.status = status;
}

void Led_SetBlinkIntervalInMs(uint32_t value)
{
    ledContext.blinkIntervalInMs = value;
}

bool Led_IsInited()
{
    return ledIsInited;
}

void Led_Init()
{
    ledContext.status = LED_STATUS_OFF;
    ledContext.blinkIntervalInMs = 500;

    ledContext.prevStatus = ledContext.status;

    ledContext.curLedStatus = false;

    ledContext.blink_t1 = 0;

    gpio_deinit(LED_PIN_ID);

    gpio_init(LED_PIN_ID);
    gpio_set_dir(LED_PIN_ID, GPIO_OUT);

    gpio_put(LED_PIN_ID, false);

    ledIsInited = true;
    sync();
}

void Led_Thread()
{
    if (!Led_IsInited())
        return;

    if (ledContext.status != ledContext.prevStatus)
    {
        volatile uint32_t newStatus = ledContext.status;

        if (newStatus == LED_STATUS_ON)
        {
            ledContext.curLedStatus = true;
            gpio_put(LED_PIN_ID, true);
        }
        else if (newStatus == LED_STATUS_OFF)
        {
            ledContext.curLedStatus = false;
            gpio_put(LED_PIN_ID, false);
        }

        ledContext.prevStatus = newStatus;
    }

    if (ledContext.status == LED_STATUS_BLINK)
    {
        uint64_t t2 = get_time_in_ms();

        if ((t2 - ledContext.blink_t1) >= ledContext.blinkIntervalInMs)
        {
            ledContext.blink_t1 = t2;

            ledContext.curLedStatus = !ledContext.curLedStatus;
            gpio_put(LED_PIN_ID, ledContext.curLedStatus);
        }
    }
}