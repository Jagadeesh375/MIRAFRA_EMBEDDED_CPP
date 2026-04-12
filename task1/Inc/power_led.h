
#include "STM32F405xx.h"
#include <cstdint>

class POWER_LED
{
private:
    volatile GPIO_TypeDef* pGPIOx;

    uint8_t green_pin;
    uint8_t orange_pin;
    uint8_t red_pin;
    uint8_t blue_pin;

    float low_th;
    float mid_th;
    float high_th;

public:
    POWER_LED(volatile GPIO_TypeDef* l_GPIOx)
    {
        pGPIOx = l_GPIOx;
        green_pin  = 12;
        orange_pin = 13;
        red_pin    = 14;
        blue_pin   = 15;
        if (pGPIOx == GPIOD)
            RCC->AHB1ENR |= (1 << 3);
        pGPIOx->MODER &= ~( (3 << (green_pin*2)) |(3 << (orange_pin*2)) | (3 << (red_pin*2)) | (3 << (blue_pin*2)) );

        pGPIOx->MODER |=  ( (1 << (green_pin*2)) | (1 << (orange_pin*2)) |(1 << (red_pin*2)) |(1 << (blue_pin*2)) );
    }

    void set_thresholds(float low, float mid, float high);

    void display(float voltage, float current);

private:
    void all_off();
    void on(uint8_t pin);
};
