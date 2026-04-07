#include <stdint.h>
typedef struct
{
    volatile uint32_t MODER; //4 byte 0x00
    volatile uint32_t OTYPER;// 0x04
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
} GPIO_RegDef_t;
typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
} RCC_RegDef_t;

#define RCC_BASE   0x40023800
#define GPIOD_BASE 0x40020C00
#define GPIOA_BASE 0x40020000

#define RCC   ((RCC_RegDef_t*) RCC_BASE) // type casting
#define GPIOD ((GPIO_RegDef_t*) GPIOD_BASE)
#define GPIOA ((GPIO_RegDef_t*) GPIOA_BASE)

class Button{
	volatile GPIO_RegDef_t *pgpiox;
	uint8_t button;
public:
	explicit Button(GPIO_RegDef_t *pgpio,uint8_t b){
		pgpiox=pgpio;
		button=b;
	}
	void init(){
		if(pgpiox==GPIOD){
			RCC->AHB1ENR|=(1<<3);
		}
		else if(pgpiox==GPIOA){
				RCC->AHB1ENR|=1;
			}
	}
	void configure(){
		pgpiox->MODER &= ~(3 << 0);
	}
	uint8_t read(){
		return (pgpiox->IDR >>button)&1;
	}
};
class Led{
private:
	volatile GPIO_RegDef_t *pgpiox;
	uint8_t pin;
public:
	Led(GPIO_RegDef_t *gpio,int pinnumber){
		pgpiox=gpio;
		pin=pinnumber;
	}
	void init(){
		if(pgpiox==GPIOD){
				RCC->AHB1ENR|=(1<<3);
				}
		else if(pgpiox==GPIOA){
				RCC->AHB1ENR|=1;
			}
	}
	void configure(){
		pgpiox->MODER &= ~(3 << pin*2);
		pgpiox->MODER|=(1<<pin*2);
	}
	 void on()
	    {
	        pgpiox->ODR |= (1 << pin);
	    }
	    void off()
	    {
	        pgpiox->ODR &= ~(1 << pin);
	    }
	    void toggle()
	    {
	        pgpiox->ODR ^= (1 << pin);
	    }
};
int main(void){
	Button b(GPIOA,0);
	b.init();
	b.configure();
	Led led12(GPIOD,12);
	led12.init();
	led12.configure();
	Led led13(GPIOD,13);
	led13.init();
	led13.configure();
	Led led14(GPIOD,14);
	led14.init();
	led14.configure();
	Led led15(GPIOD,15);
	led15.init();
	led15.configure();
	while(1){
		if(b.read()==1){
			led14.off();
			led15.off();
			led12.on();
			led13.on();
		}
		else{
			led12.off();
			led13.off();
			led14.on();
			led15.on();
		}

	}
}
