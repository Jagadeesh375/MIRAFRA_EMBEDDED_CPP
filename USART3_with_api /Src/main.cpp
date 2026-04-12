#include "STM32F407xx.h"
#include <stdint.h>

// USART3_TX → PC10
// USART3_RX → PC11structi
struct USART{
	string GPIOx_PORT;
	string USARTx;
	uint8_t USARTx_pin;
}U;
uint8_t read_Ch;

void USART_init(void);
void USART_write(uint8_t ch);
uint8_t USART_read(void);
void delayMs(int);
int main(void)
{
	U u1={"GPIOC","USART3",18};
    USART_init(&u1);   // Initialize UART

   while (1)
    {
        USART3_write('A');// 1 byte
        delayMs(500);

   }
}

void USART_init(U* u1)
{
	if(u1->USARTx=="USART3"){
    RCC->AHB1ENR |= (1 << 2);   // Enable GPIOC clock
    RCC->APB1ENR |= (1 << u1->USARTx_pin);
	// Enable USART3 clock
    // PA2→ TX
    u1->GPIOx_PORT->MODER &= ~(3 << 20);
    u1->GPIOx_PORT->MODER |=  (2<< 20);   // Alternate function -10
    // created Array :AFR[2]  , AFR[0]=AFRL ,AFR[1]= AFRH
    GPIOC->AFR[1] &= ~(0xF << 8); // AFRH =AFR[1]- Clearing 4 bits
    GPIOC->AFR[1] |=  (7 << 8);   // AF7 (USART3) // 0111 for AF7

    // PA3 → RX
    GPIOC->MODER &= ~(3 << 22);
    GPIOC->MODER |=  (2 << 22);// 10 AFR mode
    GPIOC->AFR[1] &= ~(0xF << 12);// AFRH Register
    GPIOC->AFR[1] |=  (7 << 12); // AF7 -0111
    else if(u1->USARTx=="USART2"){
        	RCC->AHB1ENR|=(1<<0);
        	 RCC->APB1ENR |= (1 << u1->USARTx_pin);
        }
        else if(u1->USARTx=="USART1"){
        	RCC->AHB1ENR |= (1 << 1);   // Enable GPIOB clock
        	    RCC->APB1ENR |= (1 << u1->USARTx_pin);
        }

    // USART settings
    USART3->BRR = 0x0683;      // 9600 baud @ 16 MHz(HSI)
    USART3->CR1 |= (1 << 3);   // TE
    //USART3->CR1 |= (1 << 2);   // RE
    USART3->CR1 |= (1 << 13);  // UE - USART enable
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

/*----------------------------------------------------------
  Delay
----------------------------------------------------------*/
void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}
