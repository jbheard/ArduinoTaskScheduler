#include "repeat.h"

void func_100ms() {
    Serial.println("in func_100ms");
    delay(100);
}

void func_200ms() {
    Serial.println("-in func_200ms");
    delay(200);
}

void func_300ms() {
    Serial.println("--in func_300ms");
    delay(300);
}

void func_400ms() {
    Serial.println("---in func_400ms");
    delay(400);
}

void func_500ms() {
    Serial.println("----in func_500ms");
    delay(500);
}

RepeatScheduler scheduler;

void setup() {
    Serial.begin(115200);

    long msDelay = 500;
    long maxTimes = -1;

    Repeat *r = new Repeat(func_200ms, msDelay, maxTimes);
    scheduler.addRepeat(r);
    r = new Repeat(func_400ms, msDelay, maxTimes);
    scheduler.addRepeat(r);
    r = new Repeat(func_100ms, msDelay, maxTimes);
    scheduler.addRepeat(r);
    r = new Repeat(func_500ms, msDelay, maxTimes);
    scheduler.addRepeat(r);
    r = new Repeat(func_300ms, msDelay, maxTimes);
    scheduler.addRepeat(r);

    // Should normalize to 100, 200, 300, 400, 500
}

void loop() {
    scheduler.update();    
}
