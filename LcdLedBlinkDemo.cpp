#include "mbed.h"
#include "TextLCD.h"

#define TOGGLE_INTERVAL 500ms

TextLCD screen(p26, p25, p24, p23, p22, p21, TextLCD::LCD16x2);

int counter = 0;

int main() {
    DigitalOut d1(LED1);
    DigitalOut d2(LED2);
    DigitalOut d3(LED3);
    DigitalOut d4(LED4);

    while (true) {
        d1 = !d1;
        d2 = !d2;
        d3 = !d3;
        d4 = !d4;

        screen.cls();
        screen.printf("HeartSync %d", counter++);
        
        ThisThread::sleep_for(TOGGLE_INTERVAL);
    }
}
