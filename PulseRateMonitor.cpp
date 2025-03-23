#include "mbed.h"
#include "TextLCD.h"

TextLCD display(p26, p25, p24, p23, p22, p21, TextLCD::LCD16x2);
AnalogIn pulseInput(p15);

float smoothing = 0.5;
float current = 0.0f;
float previous = 0.0f;

const float upperThreshold = 0.5f;
const float lowerThreshold = 0.3f;

bool peak = false;
Timer pulseTimer;
float previousTime = 0.0f;
float rate = 0.0f;

int main() {
    display.cls();
    display.printf("Heart Rate:");

    pulseTimer.start();

    while (true) {
        float input = pulseInput.read();
        current = smoothing * input + (1.0f - smoothing) * previous;
        previous = current;

        printf("Raw: %.2f, Filtered: %.2f\n", input, current);

        if (!peak && current > upperThreshold) {
            peak = true;
            float now = pulseTimer.read();
            if (previousTime > 0.0f) {
                float interval = now - previousTime;
                rate = 60.0f / interval;
            }
            previousTime = now;
        }

        if (peak && current < lowerThreshold) {
            peak = false;
        }

        display.locate(0, 1);
        display.printf("BPM: %d    ", (int)rate);

        ThisThread::sleep_for(500ms);
    }
}
