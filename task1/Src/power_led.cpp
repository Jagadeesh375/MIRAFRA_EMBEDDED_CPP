
#include "power_led.h"

void POWER_LED::set_thresholds(float low, float mid, float high)
{
    low_th  = low;
    mid_th  = mid;
    high_th = high;
}

void POWER_LED::all_off()
{
    pGPIOx->ODR &= ~( (1 << green_pin)|(1 << orange_pin)|(1 << red_pin) |(1 << blue_pin));
}
void POWER_LED::on(uint8_t pin)
{
    pGPIOx->ODR |= (1 << pin);
}
void POWER_LED::display(float voltage, float current)
{
    float power = voltage * current;

    all_off();

    if (power < low_th)
    {
        on(green_pin);
    }
    else if (power < mid_th)
    {
        on(blue_pin);
    }
    else if (power < high_th)
    {
        on(orange_pin);
    }
    else
    {
        on(red_pin);
    }
}
