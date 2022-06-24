#include "repeat.h"

void func_100ms() {
    Serial.println("in func_100ms");
    delay(100);
}

void func_200ms() {
    Serial.println("-in func_200ms");
    delay(200);
}

Repeat *repeat_100ms, *repeat_200ms;

void setup() {
    Serial.begin(115200);

    long msDelay = Milliseconds::fromSeconds(2);
    long maxTimes = 10;

    repeat_100ms = new Repeat(func_100ms, msDelay, maxTimes);
    repeat_200ms = new Repeat(func_200ms, msDelay, maxTimes);
}

void loop() {
    repeat_100ms->update();    
    repeat_200ms->update();    
}
