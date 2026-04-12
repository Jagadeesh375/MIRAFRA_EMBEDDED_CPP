/*
 * gpio_reg.cpp
 *
 *  Created on: Apr 4, 2026
 *      Author: Jeena Sijo
 */


#include "gpio_reg.h"

#define PERIPH_BASE     0x40000000U
#define AHB1_OFFSET     0x00020000U
#define AHB1_BASE       (PERIPH_BASE + AHB1_OFFSET)

#define GPIOA_BASE      (AHB1_BASE + 0x0000)
#define GPIOD_BASE      (AHB1_BASE + 0x0C00)
#define RCC_BASE        (AHB1_BASE + 0x3800)

#define RCC_AHB1ENR     (*(volatile uint32_t*)(RCC_BASE + 0x30))

#define GPIOA_MODER     (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOA_IDR       (*(volatile uint32_t*)(GPIOA_BASE + 0x10))

#define GPIOD_MODER     (*(volatile uint32_t*)(GPIOD_BASE + 0x00))
#define GPIOD_ODR       (*(volatile uint32_t*)(GPIOD_BASE + 0x14))

void GPIO::init()
{
    RCC_AHB1ENR |= (1<<0); // GPIOA
    RCC_AHB1ENR |= (1<<3); // GPIOD

    // PA0 input
    GPIOA_MODER &= ~(3 << (0*2));

    // PD12 output
    GPIOD_MODER &= ~(3 << (12*2));
    GPIOD_MODER |=  (1 << (12*2));
}

uint8_t GPIO::readPA0()
{
    return (GPIOA_IDR & (1<<0)) ? 1 : 0;
}

void GPIO::setPD12()
{
    GPIOD_ODR |= (1<<12);
}

void GPIO::resetPD12()
{
    GPIOD_ODR &= ~(1<<12);
}

