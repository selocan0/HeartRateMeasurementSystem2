#include "mbed.h"
#include <cstdio>

AnalogIn input(p15);
AnalogOut output(p18);

const int BUFFER_SIZE = 10;

int main() {
    float raw = 0.0f;
    float avg = 0.0f;
    float scaled = 0.0f;
    float step = 0.0f;
    float minimum = 1.0f;
    float maximum = 0.0f;

    float buffer[BUFFER_SIZE] = {0};
    int pos = 0;
    float total = 0;

    while (true) {
        ThisThread::sleep_for(50ms);

        raw = input.read();
        total -= buffer[pos];
        buffer[pos] = raw;
        total += buffer[pos];
        pos = (pos + 1) % BUFFER_SIZE;
        avg = total / BUFFER_SIZE;

        if (avg > maximum) maximum = avg;
        if (avg < minimum) minimum = avg;

        step = (maximum - minimum) / 8.0f;
        scaled = ((avg - minimum) / (maximum - minimum)) * 8.0f * step;

        output.write(scaled);
    }
}
