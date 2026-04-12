#include <stdint.h>
#include "STM32F405xx.h"
int main(){
	RCC->AHB1ENR|=(1<<3);//GPIOD enable clock
	GPIOD->MODER&=~(3<<30);
	GPIOD->MODER|=(1<<30);
	while(1){
	GPIOD->BSRR=(1<<15);
	 for(int i=0;i<100000;i++);
	GPIOD->BSRR=(1<<(15+16));
	 for(int i=0;i<100000;i++);
	}
}
