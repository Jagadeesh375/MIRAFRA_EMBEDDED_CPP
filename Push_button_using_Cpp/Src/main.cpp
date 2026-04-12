#include "button.h"

int main() {
    Button b(GPIOA, 0);
    LED led12(GPIOD, 12);
    LED led13(GPIOD, 13);
    LED led14(GPIOD, 14);
    LED led15(GPIOD, 15);

    while (1) {
        if (b.read() == 1) {
            led14.off();
            led15.off();
            led12.on();
            led13.on();
        } else {
            led12.off();
            led13.off();
            led14.on();
            led15.on();
        }
    }
}
