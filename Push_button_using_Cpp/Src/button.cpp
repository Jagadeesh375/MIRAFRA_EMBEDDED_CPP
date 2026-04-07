#include "button.h"

Button::Button(GPIO_TypeDef *pGpio, uint8_t b) {
    pGpiox = pGpio;
    pin = b;
    if (pGpiox == GPIOA) {
        RCC->AHB1ENR |= (1 << 0);
    } else if (pGpiox == GPIOD) {
        RCC->AHB1ENR |= (1 << 3);
    }
    pGpiox->MODER &= ~(3 << (pin * 2));
}

uint8_t Button::read() {
    return (pGpiox->IDR >> pin) & 1;
}

LED::LED(GPIO_TypeDef *pgpio, uint8_t p) : pgpiox(pgpio), pin(p) {
    if (pgpiox == GPIOD) {
        RCC->AHB1ENR |= (1 << 3);
    } else if (pgpiox == GPIOA) {
        RCC->AHB1ENR |= 1;
    }
    pgpiox->MODER &= ~(3 << (pin * 2));
    pgpiox->MODER |=  (1 << (pin * 2));
}

void LED::on() {
    pgpiox->ODR |= (1 << pin);
}

void LED::off() {
    pgpiox->ODR &= ~(1 << pin);
}

void LED::toggle() {
    pgpiox->ODR ^= (1 << pin);
}
