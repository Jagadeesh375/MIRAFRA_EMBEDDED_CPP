
#include <stdint.h>
#include "STM32F405xx.h"
#include "power_led.h"

int main(void)
{
    POWER_LED led((GPIO_TypeDef*)GPIOD_BASE);

    led.set_thresholds(5, 10, 20);

    float value = 1.0;

    while (1)
    {

        value += 1.0;


        if (value > 10)
            value = 1.0;


        led.display(value, value);


        for ( int i = 0; i < 10000; i++);
    }
}
