#include<stdint.h>
#include"STM32F407xx.h"
constexpr uint8_t count=10;
extern "C" void USART3_write(const char *ptr)
{
	while(*ptr!='\0'){
    while (!(USART3->SR & (1U << 7)));   // TXE
    USART3->DR = *ptr++;
	}
    while (!(USART3->SR & (1U << 6)));// TC
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
// Observer interface
class Observer {
public:
    virtual void update(float temperature) = 0;
};

// Subject class (TemperatureSensor)
class TemperatureSensor {
    float temperature;
    Observer* observers[count] = {nullptr};
    uint8_t obscount=0;

public:
    void attach(Observer* observer) {
        if(obscount < count) {
            observers[obscount++] = observer;
        }
    }

    /*void detach(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
        std::cout << "Detached" << std::endl;
    }
*/
    void setTemperature(float newTemperature) {
        if (temperature != newTemperature) {
            temperature = newTemperature;
            notify();
        }
    }

    void notify() {
        for(int i=0;i<obscount;i++){
        	observers[i]->update(temperature);
        }
    }
};

// Concrete Observer: DisplaySystem
class DisplaySystem : public Observer {
public:
    void update(float temperature) override {
        USART3_write("Display updated: Temperature is  °C\n\r");
    }
};

// Concrete Observer: AlarmSystem
class AlarmSystem : public Observer {
public:
    void update(float temperature) override {
        if (temperature > 30.0) { // Example threshold for high temperature
            USART3_write("Alarm triggered! Temperature too high:  temperature °C\n");
        }
    }
};



// Main program to demonstrate the Observer pattern
int main() {
    // Create the subject (sensor)
    TemperatureSensor sensor;
    USART3_init();

    // Create observers
    DisplaySystem display;
    AlarmSystem alarm;

    // Attach observers to the sensor
    sensor.attach(&display);
    sensor.attach(&alarm);

    // Simulate temperature changes
    USART3_write( "Setting temperature to 25°C\n\r");
    sensor.setTemperature(25.0); // Normal temperature, only display updates

    USART3_write("\nSetting temperature to 35°C\n\r");
    sensor.setTemperature(35.0); // High temperature, triggers both display and alarm
  //  sensor.detach(&alarm);
    return 0;
}
