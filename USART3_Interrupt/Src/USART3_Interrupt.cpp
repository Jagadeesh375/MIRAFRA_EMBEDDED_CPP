#include "STM32F407xx.h"
#include <stdint.h>
#include<cstdio>
uint8_t read_Ch;

inline volatile uint32_t *NVIC_ISER1 = reinterpret_cast<volatile uint32_t*>(0xE000E104);
inline volatile uint32_t *NVIC_ICPR1 = reinterpret_cast<volatile uint32_t*>(0xE000E284);

void USART3_init(void);
void LED_init(void);

int main(void)
{
    LED_init();
    USART3_init();
    while (1) { }
}

void LED_init(void)
{
    RCC->AHB1ENR|=(1 << 3);

    GPIOD->MODER&=~(0xFF << 24);    // Clear bits for PD12–PD15
    //output mode 01
    GPIOD->MODER|=(0x55 << 24);    //we get decimal value=85,after that hex value=0x55


    GPIOD->ODR&=~(0xF << 12);     // Turn all LEDs off initially
}

void USART3_init(void)
{
    RCC->AHB1ENR|=(1 << 2);         // Enable GPIOC clock
    RCC->APB1ENR|=(1 << 18);        // Enable USART3 clock

    // PC10 → TX (AF7)
    GPIOC->MODER&=~(3 << 20);
    GPIOC->MODER|=(2 << 20);
    GPIOC->AFR[1]&=~(0xF << 8);
    GPIOC->AFR[1]|=(7 << 8);

    // PC11 → RX (AF7)
    GPIOC->MODER&=~(3 << 22);
    GPIOC->MODER|=(2 << 22);
    GPIOC->AFR[1]&=~(0xF << 12);
    GPIOC->AFR[1]|=(7 << 12);

    USART3->BRR=0x0683;          // 9600 baud
    USART3->CR1|=(1 << 2);         // RE
    USART3->CR1|=(1 << 5);         // RXNEIE
    USART3->CR1|=(1 << 13);        // UE

    *NVIC_ICPR1|=(1 << 7);
    *NVIC_ISER1|=(1 << 7);//USART3 IRQ number is 39
}

extern "C" void USART3_IRQHandler(void)
{
    if (USART3->SR & (1 << 5))//check RXNE value
    {
        read_Ch = USART3->DR; // Read clears RXNE
        printf("In USART3 handler");
        GPIOD->ODR &= ~(0xF << 12);
        switch (read_Ch)
        {
            case 'A': case 'a':
                GPIOD->ODR|=(1 << 12);   // Green LED ON
                break;
            case 'B': case 'b':
                GPIOD->ODR|=(1 << 13);   // Orange LED ON
                break;
            case 'C': case 'c':
                GPIOD->ODR|=(1 << 14);   // Red LED ON
                break;
            case 'D': case 'd':
                GPIOD->ODR|=(1 << 15);   // Blue ON
                break;
            default:
            	GPIOD->ODR|=(0XF<<12);
                break;                     // Unknown char → all LEDs off
        }
    }

}
