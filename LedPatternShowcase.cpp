#include "mbed.h"

#define REG_NOOP         0x00
#define REG_DIGIT0       0x01
#define REG_DIGIT1       0x02
#define REG_DIGIT2       0x03
#define REG_DIGIT3       0x04
#define REG_DIGIT4       0x05
#define REG_DIGIT5       0x06
#define REG_DIGIT6       0x07
#define REG_DIGIT7       0x08
#define REG_DECODE_MODE  0x09
#define REG_INTENSITY    0x0a
#define REG_SCAN_LIMIT   0x0b
#define REG_SHUTDOWN     0x0c
#define REG_DISPLAY_TEST 0x0f

#define LOW  0
#define HIGH 1

SPI matrixSPI(p5, NC, p7);
DigitalOut cs(p8);
DigitalOut ledStatus(LED1);

char patternA[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
char patternB[8] = { 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff };
char patternC[8] = { 0x04, 0x15, 0x0e, 0x1f, 0x0e, 0x15, 0x04, 0x00 };

void writeToMatrix(int reg, int val) {
    cs = LOW;
    matrixSPI.write(reg);
    matrixSPI.write(val);
    cs = HIGH;
}

void displayPattern(char *data) {
    for (int i = 0; i < 8; i++) {
        writeToMatrix(i + 1, data[i]);
    }
}

void initMatrix() {
    matrixSPI.format(8, 0);
    matrixSPI.frequency(100000);
    writeToMatrix(REG_SCAN_LIMIT, 0x07);
    writeToMatrix(REG_DECODE_MODE, 0x00);
    writeToMatrix(REG_SHUTDOWN, 0x01);
    writeToMatrix(REG_DISPLAY_TEST, 0x00);
    writeToMatrix(REG_INTENSITY, 0x08);
    for (int i = 1; i <= 8; i++) {
        writeToMatrix(i, 0);
    }
}

void clearMatrix() {
    for (int i = 1; i <= 8; i++) {
        writeToMatrix(i, 0);
    }
}

int main() {
    initMatrix();
    while (true) {
        displayPattern(patternA);
        ledStatus = HIGH;
        thread_sleep_for(1000);

        ledStatus = LOW;
        displayPattern(patternB);
        thread_sleep_for(1000);

        ledStatus = HIGH;
        displayPattern(patternC);
        thread_sleep_for(1000);

        ledStatus = LOW;
        thread_sleep_for(1000);
        clearMatrix();
    }
}
