#include <cstdint>
#include<string>
#include<array>
constexpr uint32_t RCC_AHB1ENR_ADD=0x40023830;
constexpr uint32_t GPIOD_MODER_ADD=0x40020C00;
constexpr uint32_t GPIOD_ODR_ADD=0x40020C14;
constexpr uint32_t GPIOA_IDR_ADD=0x40020010;
template<uint32_t address>
struct Register{
	static volatile uint32_t & value(){
		return *reinterpret_cast<uint32_t*>(address);
	}
};
std::array<int,4>LED={12,13,14,15};
class GPIO{
public:
	static void clock_enable(const std::string& s)noexcept{
		auto &res=Register<RCC_AHB1ENR_ADD>::value();
		if(s=="GPIOA"){
			res|=(1<<0);
		}
		else if(s=="GPIOD"){
			res|=(1<<3);
		}
	}
	static void set_output(uint32_t pin)noexcept{
		auto& moder=Register<GPIOD_MODER_ADD>::value();
		moder&=~(3<<(pin*2));
		moder|=(1<<pin*2);
	}
	static uint8_t read(uint8_t pin)noexcept{
			return (Register<GPIOA_IDR_ADD>::value()>>pin)&1;
	}
	static void led_on(uint32_t pin)noexcept{
		Register<GPIOD_ODR_ADD>::value()|=(1 << pin);
	    }
	    static void led_off(uint32_t pin)noexcept{
	    	Register<GPIOD_ODR_ADD>::value()&=~(1 << pin);
	    }
	    static void all_blink(){
	    	for(auto& i:LED){
	    		led_on(i);
	    	}
	    }
};
void delay()
{
    for (volatile std::uint32_t i = 0; i < 10000000; ++i);
}
int main(){
	constexpr uint32_t BUTTON_PIN=0;
	GPIO::clock_enable("GPIOA");
	GPIO::clock_enable("GPIOD");
	for(auto& i:LED){
	GPIO::set_output(i);
	}
	GPIO::all_blink();
	delay();
while(true){
	if(GPIO::read(BUTTON_PIN)==1){
		 GPIO::led_on(12);
		 GPIO::led_on(13);
		 delay();
		 GPIO::led_off(12);
		 GPIO::led_off(13);
	}
	else{
		 GPIO::led_on(14);
		GPIO::led_on(15);
		delay();
		GPIO::led_off(12);
		GPIO::led_off(13);
	}

}

}
