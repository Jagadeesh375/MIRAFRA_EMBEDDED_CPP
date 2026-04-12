#include "STM32F407xx.h"
#include <stdint.h>

// USART3_TX → PC10
// USART3_RX → PC11
// LED → PD12

void USART3_init(void);
uint8_t USART3_read(void);
void LED_init(void);

/*----------------------------------------------------------
  MAIN
----------------------------------------------------------*/
int main(void)
{
    USART3_init();
    LED_init();

    while (1)
    {
        uint8_t ch = USART3_read(); // wait for input

        if (ch == '1')
        {
            GPIOD->BSRR = (1 << 12);         // LED ON
        }
        else if (ch == '0')
        {
            GPIOD->BSRR = (1 << (12 + 16));  // LED OFF
        }
    }
}

/*----------------------------------------------------------
  USART3 Initialization
----------------------------------------------------------*/
void USART3_init(void)
{
    RCC->AHB1ENR |= (1 << 2);   // Enable GPIOC clock
    RCC->APB1ENR |= (1 << 18);  // Enable USART3 clock

    // PC10 → TX (AF7)
    GPIOC->MODER &= ~(3 << 20);
    GPIOC->MODER |=  (2 << 20);
    GPIOC->AFR[1] &= ~(0xF << 8);
    GPIOC->AFR[1] |=  (7 << 8);

    // PC11 → RX (AF7)
    GPIOC->MODER &= ~(3 << 22);
    GPIOC->MODER |=  (2 << 22);
    GPIOC->AFR[1] &= ~(0xF << 12);
    GPIOC->AFR[1] |=  (7 << 12);

    //  Important: Enable pull-up for RX
    GPIOC->PUPDR &= ~(3 << 22);
    GPIOC->PUPDR |=  (1 << 22);

    // USART configuration
    USART3->BRR = 0x0683;       // 9600 baud @ 16 MHz

    USART3->CR1 |= (1 << 3);    // TE (Transmit enable)
    USART3->CR1 |= (1 << 2);    // RE (Receive enable)
    USART3->CR1 |= (1 << 13);   // UE (USART enable)
}

/*----------------------------------------------------------
  Read one byte
----------------------------------------------------------*/
uint8_t USART3_read(void)
{
    while (!(USART3->SR & (1 << 5))); // Wait for RXNE
    return USART3->DR;
}

/*----------------------------------------------------------
  LED Initialization (PD12)
----------------------------------------------------------*/
void LED_init(void)
{
    RCC->AHB1ENR |= (1 << 3); // Enable GPIOD clock

    GPIOD->MODER &= ~(3 << (12 * 2));
    GPIOD->MODER |=  (1 << (12 * 2)); // Output mode
}
