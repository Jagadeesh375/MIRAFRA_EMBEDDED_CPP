#include <cstdint>
#include<string>
//using namespace std;
constexpr uint32_t RCC_AHB1ENR_ADD=0x40023830;
constexpr uint32_t GPIOD_MODER_ADD=0x40020C00;
constexpr uint32_t GPIOD_ODR_ADD=0x40020C14;
constexpr uint32_t GPIOA_IDR_ADD=0x40020010;

inline volatile uint32_t& RCC_AHB1ENR=*reinterpret_cast<volatile uint32_t *>(RCC_AHB1ENR_ADD);
inline volatile uint32_t& GPIOD_MODER=*reinterpret_cast<volatile uint32_t *>(GPIOD_MODER_ADD);
inline volatile uint32_t& GPIOD_ODR=*reinterpret_cast<volatile uint32_t *>(GPIOD_ODR_ADD);
inline volatile uint32_t& GPIOA_IDR=*reinterpret_cast<volatile uint32_t *>(GPIOA_IDR_ADD);

class GPIO{
public:
	static void clock_enable(const std::string& s){
		if(s=="GPIOA"){
			RCC_AHB1ENR|=(1<<0);
		}
		else if(s=="GPIOD"){
			RCC_AHB1ENR|=(1<<3);
		}
	}
	static
	static void set_output(uint32_t pin){
		GPIOD_MODER&=~(3<<(pin*2));
		GPIOD_MODER&=~(3<<pin*2);
	}
	static uint8_t read(uint8_t pin){
			return (GPIOA_IDR>>pin)&1;
	}
	static void led_on(uint32_t pin) {
	        GPIOD_ODR |= (1 << pin);
	    }
	    static void led_off(uint32_t pin) {
	        GPIOD_ODR &= ~(1 << pin);
	    }


};
void delay()
{
    for (volatile std::uint32_t i = 0; i < 100000; ++i);
}
int main(){
	constexpr uint32_t LED_PIN=12;
	constexpr uint32_t BUTTON_PIN=0;
	GPIO::clock_enable("GPIOA");
	GPIO::clock_enable("GPIOD");
	GPIO::set_output(LED_PIN);
while(true){
	if(GPIO::read(BUTTON_PIN)==1){
		 GPIO::led_on(LED_PIN);
		 delay();
	}
	else{
		GPIO::led_off(LED_PIN);
		delay();
	}

}

}
