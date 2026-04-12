#include <stdint.h>
#include<STM32F407xx.h>
int main(void)
{
    /* RCC Registers */
    //uint32_t *pRCC_AHB1ENR = (uint32_t*)0x40023830;
   // uint32_t *pRCC_APB1ENR = (uint32_t*)0x40023840;

    /* GPIOC Registers */
    //uint32_t *pGPIOC_MODER = (uint32_t*)0x40020800;
    //uint32_t *pGPIOC_ODR   = (uint32_t*)0x40020814;

    /* TIM2 Registers */
   /* uint32_t *pTIM2_CR1 = (uint32_t*)0x40000000;
    uint32_t *pTIM2_SR  = (uint32_t*)0x40000010;
    uint32_t *pTIM2_CNT = (uint32_t*)0x40000024;
    uint32_t *pTIM2_PSC = (uint32_t*)0x40000028;
    uint32_t *pTIM2_ARR = (uint32_t*)0x4000002C;*/

    /* 1. Enable clocks */
   // *pRCC_AHB1ENR |= (1 << 2);  // GPIOC enable
    //*pRCC_APB1ENR |= (1 << 0);  // TIM2 enable
    RCC->AHB1ENR|=(1<<3);
    RCC->APB1ENR|=(1<<0);

    /* 2. Configure PC6 as output */
//    *pGPIOC_MODER &= ~(3 << (6 * 2));  // Clear bits
  //  *pGPIOC_MODER |=  (1 << (6 * 2));  // Set output mode
    GPIOD->MODER&=~(3<<30);
    GPIOD->MODER|=(1<<30);

    /* 3. Timer configuration for 1 second delay */

    // System clock = 16 MHz
    // Step 1: Prescaler → 16 MHz / 1600 = 10 kHz
   // *pTIM2_PSC = 1600 - 1;
    TIM2->PSC=1600-1;
    // Step 2: ARR → 10,000 ticks → 1 second
//    *pTIM2_ARR = 10000 - 1;
TIM2->ARR=10000-1;
    /* 4. Reset counter */
    //*pTIM2_CNT = 0;
TIM2->CNT=0;

    /* 5. Enable timer */
    //*pTIM2_CR1 |= (1 << 0);
TIM2->CR1|=(1<<0);
    while(1)
    {
        /* Wait until update event (UIF = 1) */
        while(!(TIM2->SR & (1 << 0)));

        /* Clear UIF flag */
        TIM2->SR &= ~(1 << 0);

        /* Toggle LED (PC6) */
        GPIOD->ODR ^= (1 << 15);
    }
}
