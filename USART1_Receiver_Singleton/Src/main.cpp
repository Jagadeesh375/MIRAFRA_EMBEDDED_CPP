#include "STM32F407xx.h"
#include <stdint.h>

class LEDController {
public:
    static LEDController& getInstance() {
        static LEDController instance;
        return instance;
    }

    LEDController(const LEDController&) = delete;
    LEDController& operator=(const LEDController&) = delete;

    void run() {
        while (1) {
            read_Ch = USART1_read();

            if (read_Ch == '1') {
                GPIOB->BSRR = (1 << 4);
                delayMs(1000);
                GPIOB->BSRR = (1 << (4 + 16));
            }
            else if (read_Ch == '2') {
                GPIOB->BSRR = (1 << 3);
                delayMs(1000);
                GPIOB->BSRR = (1 << (3 + 16));
            }
            else if (read_Ch == '3') {
                GPIOE->BSRR = (1 << 2);
                delayMs(1000);
                GPIOE->BSRR = (1 << (2 + 16));
            }
            else if (read_Ch == '4') {
                GPIOE->BSRR = (1 << 5);
                delayMs(1000);
                GPIOE->BSRR = (1 << (5 + 16));
            }

            read_Ch = 0;
        }
    }

private:
    uint8_t read_Ch = 0;
    uint8_t pb = 1;

    LEDController() {
        USART1_init();
        LED_init();
    }

    void USART1_init() {
        RCC->AHB1ENR |= (1 << 1);
        RCC->APB2ENR |= (1 << 4);

        // PB6 -> TX
        GPIOB->MODER &= ~(3 << 12);
        GPIOB->MODER |=  (1 << 13);
        GPIOB->AFR[0] &= ~(0xF << 24);
        GPIOB->AFR[0] |=  (7 << 24);

        // PB7 -> RX
        GPIOB->MODER &= ~(3 << 14);
        GPIOB->MODER |=  (1 << 15);
        GPIOB->AFR[0] &= ~(0xF << 28);
        GPIOB->AFR[0] |=  (7 << 28);

        // USART1 settings
        USART1->BRR    =  0x0683;
        USART1->CR1   |= (1 << 2);   // RE
        USART1->CR1   |= (1 << 13);  // UE
    }

    void LED_init() {
        RCC->AHB1ENR |= (1 << 1);  // GPIOB
        RCC->AHB1ENR |= (1 << 4);  // GPIOE

        // PB4 output
        GPIOB->MODER &= ~(3 << (4 * 2));
        GPIOB->MODER |=  (1 << (4 * 2));

        // PB3 output
        GPIOB->MODER &= ~(3 << (3 * 2));
        GPIOB->MODER |=  (1 << (3 * 2));

        // PE2 output
        GPIOE->MODER &= ~(3 << (2 * 2));
        GPIOE->MODER |=  (1 << (2 * 2));

        // PE5 output
        GPIOE->MODER &= ~(3 << (5 * 2));
        GPIOE->MODER |=  (1 << (5 * 2));
    }

    uint8_t USART1_read() {
        while (!(USART1->SR & (1 << 5)));  // Wait for RXNE
        return USART1->DR;
    }

    void delayMs(int n) {
        for (; n > 0; n--)
            for (volatile int i = 0; i < 2000; i++);
    }
};

int main() {
    LEDController::getInstance().run();
}
