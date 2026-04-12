#include "STM32F405xx.h"
#include <cstdint>
class Button {
    GPIO_TypeDef *pGpiox;
    uint8_t pin;
public:
    explicit Button(GPIO_TypeDef *pGpio, uint8_t b);
    uint8_t read();
};

class LED {
private:
    GPIO_TypeDef *pgpiox;
    uint8_t pin;
public:
    explicit LED(GPIO_TypeDef *pgpio, uint8_t p);
    void on();
    void off();
    void toggle();
};
