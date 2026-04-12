#include <stdint.h>
#include <stdio.h>
#include<STM32F407xx.h>
/* ---------------- C LINKAGE (VERY IMPORTANT) ---------------- */
extern "C" void UsageFault_Handler(void);
extern "C" void HardFault_Handler(void);
extern "C" void MemManage_Handler(void);
extern "C" void BusFault_Handler(void);
extern "C" void UsageFault_Handler_c(uint32_t *pMSP);

/* ---------------- DIVIDE FUNCTION ---------------- */
int fun_divide(int x , int y)
{
    return x / y;
}
void USART3_init()
{
    // Enable clocks
    RCC->AHB1ENR |= (1U << 2);   // GPIOC
    RCC->APB1ENR |= (1U << 18);  // USART3


    // PC10 -> USART3_TX
    GPIOC->MODER &= ~(3U << 20);
    GPIOC->MODER |=  (2U << 20);
    GPIOC->AFR[1] &= ~(0xFU << 8);
    GPIOC->AFR[1] |=  (7U << 8);

    // PC11 -> USART3_RX
    GPIOC->MODER &= ~(3U << 22);
    GPIOC->MODER |=  (2U << 22);
    GPIOC->AFR[1] &= ~(0xFU << 12);
    GPIOC->AFR[1] |=  (7U << 12);

    // USART3 settings
    USART3->BRR = 0x0683;      // 9600 baud @ 16 MHz
    USART3->CR1 |= (1U << 3);  // TE
    USART3->CR1 |= (1U << 2);  // RE
    USART3->CR1 |= (1U << 13); // UE
}
extern "C" void USART3_write(uint8_t ch)
{
    while (!(USART3->SR & (1U << 7)));   // TXE
    USART3->DR = ch;
    while (!(USART3->SR & (1U << 6)));   // TC
}

/*extern "C" void USART3_write_String(uint8_t ch){
	USART3_write(ch);
}*/



/* ---------------- MAIN ---------------- */
int main(void)
{
    /* Enable configurable faults */
	USART3_init();
    uint32_t *pSHCSR = (uint32_t*)0xE000ED24; // SHCSR from ARM CORTEX Ref manual

    *pSHCSR |= (1 << 16); // MemManage
    *pSHCSR |= (1 << 17); // BusFault
    *pSHCSR |= (1 << 18); // UsageFault ENABLED

    /* Enable divide-by-zero trap */
    uint32_t *pCCR = (uint32_t*)0xE000ED14;
    *pCCR |= (1 << 4);// enable DIV_0 TRAP

    /* Force runtime division (avoid optimization) */
    volatile int a = 10;
    volatile int b = 0;

    fun_divide(a, b);   //  UsageFault

    while(1);
}

/* ---------------- FAULT HANDLERS ---------------- */

extern "C" void HardFault_Handler(void)
{

	__asm("BKPT #0");   // Debugger stops
    while(1);
}

extern "C" void MemManage_Handler(void)
{
    __asm("BKPT #0");
    while(1);
}

extern "C" void BusFault_Handler(void)
{
    __asm("BKPT #0");
    while(1);
}

/* ---------------- NAKED USAGE FAULT HANDLER ---------------- */
extern "C" __attribute__((naked)) void UsageFault_Handler(void)
{
    __asm volatile(
        "MRS r0, MSP        \n"  // Pass MSP to C handler
        "B UsageFault_Handler_c \n"
    );
}
/* ---------------- C HANDLER ---------------- */
extern "C" void UsageFault_Handler_c(uint32_t *pBaseStackFrame)
{
    uint32_t *pUFSR = (uint32_t*)0xE000ED2A;//Usage fault status register (UFSR)

    printf("\n--- UsageFault Occurred ---\n\r");
    printf("UFSR = %lx\n\r", (*pUFSR) & 0xFFFF); // 1111 1111 1111 1111
    uint8_t status= (*pUFSR & (1<<9));
   // printf("status=%lx\n",Usart3_write(status));;
    printf("status=%d\n\r",status);
    printf("R0  = %lx\n\r", pBaseStackFrame[0]);
    printf("R1  = %lx\n\r", pBaseStackFrame[1]);
    printf("R2  = %lx\n\r", pBaseStackFrame[2]);
    printf("R3  = %lx\n\r", pBaseStackFrame[3]);
    printf("R12 = %lx\n\r", pBaseStackFrame[4]);
    printf("LR  = %lx\n\r", pBaseStackFrame[5]);
    printf("PC  = %lx\n\r", pBaseStackFrame[6]);
    printf("XPSR= %lx\n\r", pBaseStackFrame[7]);

    /*  Reliable debug stop */
    __asm("BKPT #0");

    while(1);
}
