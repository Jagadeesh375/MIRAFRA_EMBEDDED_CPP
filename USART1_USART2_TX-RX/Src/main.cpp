#include <stdint.h>
#include<STM32F407xx.h>
void LED_init(){
	RCC->AHB1ENR=(1<<1);//GPIOB clock is enable
	RCC->AHB1ENR=(1<<4);//GPIOE clock is enable

	GPIOB->MODER&=~(3<<(5*2));
	GPIOB->MODER&=~(3<<(9*2));

	GPIOE->MODER&=~(3<<(2*2));
	GPIOE->MODER&=~(3<<(5*2));
}

void led_on(const string& port,uint8_t pin){
	if(port=="PORTB")
	GPIOB->BSRR=(1<<pin);
	else if(port=="PORTE"){
		GPIOE->BSRR=(1<<pin);
	}
}
void led_on(const string& port,uint8_t pin){
	if(port=="PORTB")
	GPIOB->BSRR=(1<<(pin+16));
	else if(port=="PORTE"){
		GPIOE->BSRR=(1<<(pin+16));
	}
}
void USART1_init(){
	RCC->APB2ENR|=(1<<4);//enable USART1
	RCC->AHB1ENR|=(1<<0);//enable PORTA clock
	//TX PA9
	GPIOA->MODER&=~(3<<18);
	GPIOA->MODER|=(2<<18);
	GPIOA->AFR[1]&=~(0xF<<4);
	GPIOA->AFR[1]|=(0x7<<4);
//RX PA10
	GPIOA->MODER&=~(3<<20);
	GPIOA->MODER|=(2<<20);
	GPIOA->AFR[1]&=~(0xF<<8);
	GPIOA->AFR[1]|=(0X7<<8);
	    USART1->BRR = 0x0683;         // 9600 baud
	    USART1->CR1 |= (1 << 3);      // TE
	    //USART1->CR1 |= (1 << 2);      // RE
	    USART1->CR1 |= (1 << 13); 	//UE
}
void USART2_init(){
	RCC->AHB1ENR |= (1 << 0);    // Enable GPIOA clock
	    RCC->APB1ENR |= (1 << 17);   // Enable USART2 clock

	    // PA2 → TX (AF7)
	    GPIOA->MODER &= ~(3 << 4);
	    GPIOA->MODER |=  (2 << 4);
	    GPIOA->AFR[0] &= ~(0xF << 8);
	    GPIOA->AFR[0] |=  (7 << 8);

	    // PA3 → RX (AF7)
	    GPIOA->MODER &= ~(3 << 6);
	    GPIOA->MODER |=  (2 << 6);
	    GPIOA->AFR[0] &= ~(0xF << 12);
	    GPIOA->AFR[0] |=  (7 << 12);

	    // Pull-up on PA3 (RX)
	    GPIOA->PUPDR &= ~(3 << 6);
	    GPIOA->PUPDR |=  (1 << 6);

	    USART2->BRR = 0x0683;         // 9600 baud
	  //  USART2->CR1 |= (1 << 3);      // TE
	    USART2->CR1 |= (1 << 2);      // RE
	    USART2->CR1 |= (1 << 13);     // UE
}
/*void USART1_write(uint8_t ch){
	while(!(USART1->SR&(1<<7)));//wait for TXE
	USART1->DR=ch;
	while(!(USART1->SR &(1<<6)));//wait for TC
}*/
uint8_t USART1_read(){
	while(!(USART1->SR&(1<<5)));//wait for RXE
	return USART1->DR;
}
void delay(){
	 for(volatile int i = 0; i < 500000; i++);
}
int main(void)
{
	uint8_t read_ch;
	USART1_init();
	USART2_init();
	LED_init();
	while(1){
		read_ch=USART2_read();
		if(read_ch=='1'){
			led_on("PORTB",5);
			delay();
		}
		else if(read_ch=='2'){
			led_on("PORTB",9);
			delay();
		}
		else if(read_ch=='3'){
			lead_on("PORTE",2);
			delay();
		}
		else if(read_ch=='4'){
				lead_on("PORTE",5);
				delay();
			}

	}

}
