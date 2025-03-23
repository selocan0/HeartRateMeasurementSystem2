#include "mbed.h"

#define REG_NOOP     0x00
#define REG_DIGIT_0  0x01
#define REG_DIGIT_1  0x02
#define REG_DIGIT_2  0x03
#define REG_DIGIT_3  0x04
#define REG_DIGIT_4  0x05
#define REG_DIGIT_5  0x06
#define REG_DIGIT_6  0x07
#define REG_DIGIT_7  0x08
#define REG_DECODE   0x09
#define REG_BRIGHT   0x0A
#define REG_SCAN     0x0B
#define REG_SHUT     0x0C
#define REG_TEST     0x0F

SPI spi(p5, NC, p7);
DigitalOut chipSelect(p8);
DigitalOut indicator(LED1);

char diagonal[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
char square[8]   = { 0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF };
char star[8]     = { 0x04, 0x15, 0x0E, 0x1F, 0x0E, 0x15, 0x04, 0x00 };

void send(int reg, int val) {
    chipSelect = 0;
    spi.write(reg);
    spi.write(val);
    chipSelect = 1;
}

void show(char* frame) {
    for (int i = 0; i < 8; i++) {
        send(i + 1, frame[i]);
    }
}

void initDisplay() {
    spi.format(8, 0);
    spi.frequency(100000);
    send(REG_SCAN, 0x07);
    send(REG_DECODE, 0x00);
    send(REG_SHUT, 0x01);
    send(REG_TEST, 0x00);
    send(REG_BRIGHT, 0x08);
    for (int i = 1; i <= 8; i++) {
        send(i, 0);
    }
}

void resetDisplay() {
    for (int i = 1; i <= 8; i++) {
        send(i, 0);
    }
}

int main() {
    initDisplay();

    while (true) {
        show(diagonal);
        indicator = 1;
        thread_sleep_for(1000);

        indicator = 0;
        show(square);
        thread_sleep_for(1000);

        indicator = 1;
        show(star);
        thread_sleep_for(1000);

        indicator = 0;
        thread_sleep_for(1000);
        resetDisplay();
    }
}
