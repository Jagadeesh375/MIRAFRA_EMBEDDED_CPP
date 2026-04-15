#include "STM32F407xx.h"
#include <stdint.h>


// NVIC
inline volatile uint32_t *NVIC_ISER = (uint32_t*)0xE000E100;
inline volatile uint32_t *NVIC_ICPR = (uint32_t*)0xE000E280;

//void TIM2_init(void);
//void delaySec(uint32_t sec);

void USART3_init(void);
void USART3_write(uint8_t ch);
uint8_t USART3_read(void);
void USART3_string(char *ch);


extern "C" void USART3_IRQHandler(void)
{
    if (USART3->SR & (1 << 5))   // RXNE
    {
        uint8_t data = USART3->DR;   // clears RXNE
        USART3_write(data);


        USART3_string("\n\rRXNE Interrupt\n\r");
        //delaySec(2);
    }
}






int main(void)
{
    USART3_init();   // Initialize UART
    //TIM2_init();

   while (1)
    {
        //USART3_write('A');// 1 byte
        //delaySec(2);

   }
}

void USART3_init(void)
{
    RCC->AHB1ENR |= (1 << 2);   // Enable GPIOC clock
    RCC->APB1ENR |= (1 << 18);  // Enable USART3 clock

    // PC10 → TX
    GPIOC->MODER &= ~(3 << 20); // pin10 =pin*2, claered the bits
    GPIOC->MODER |=  (1 << 21);   // Alternate function -10
    // created Array :A[2]  , AFR[0]=AFRL ,AFR[1]= AFRH
    GPIOC->AFR[1] &= ~(0xF << 8); // AFRH =AFR[1]- Clearing 4 bits
    GPIOC->AFR[1] |=  (7 << 8);   // AF7 (USART3) // 0111 for AF7

    // PC11 → RX
    GPIOC->MODER &= ~(3 << 22);
    GPIOC->MODER |=  (1 << 23);// 10 AFR mode
    GPIOC->AFR[1] &= ~(0xF << 12);// AFRH Register
    GPIOC->AFR[1] |=  (7 << 12); // AF7 -0111

    // USART settings
    USART3->BRR = 0x0683;      // 9600 baud @ 16 MHz(HSI)
    USART3->CR1 |= (1 << 3);   // TE
    USART3->CR1 |= (1 << 2);   // RE
    USART3->CR1 |= (1 << 13);  // UE - USART enable

   //USART3->CR1 |=(1<<7); //TXE interrupt enable
    //USART3->CR1 |=(1<<6); //TC interrupt enable
   USART3->CR1 |=(1<<5); //RXNE interrupt enable

   NVIC_ICPR[1] |= (1 << 7);   // clear pending IRQ39
   NVIC_ISER[1] |= (1 << 7);   // enable IRQ39

}

/*----------------------------------------------------------
  Write one byte
----------------------------------------------------------*/
void USART3_write(uint8_t ch) //ch ='A'
{
    // Wait until DR empty
    while (!(USART3->SR & (1 << 7))); // TXE =0 while(!0) - while loop(true)
    //TXE=1 Data in DR register will be given to shift register

    USART3->DR = ch; // new data will be loaded to DR

    // Wait until transmission complete
    while (!(USART3->SR & (1 << 6))); // TC=0 while(!0) while(T) waiting
}
uint8_t USART3_read(void)
{
    while (!(USART3->SR & (1 << 5))); // RXNE
    return USART3->DR;
}

/*----------------------------------------------------------*/
void USART3_string(char *ch)
{
    for(int i=0; ch[i]; i++)
    {
        while (!(USART3->SR & (1 << 7)));
        USART3->DR = ch[i];
        while (!(USART3->SR & (1 << 6)));

    }


}
