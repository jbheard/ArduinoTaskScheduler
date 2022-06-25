#include "task.h"

void func_100ms() {
    Serial.println("in func_100ms");
    delay(100);
}

void func_200ms() {
    Serial.println("-in func_200ms");
    delay(200);
}

Task *task_100ms, *task_200ms;

void setup() {
    Serial.begin(115200);

    long msDelay = Milliseconds::fromSeconds(2);
    long maxTimes = 10;

    task_100ms = new Task(func_100ms, msDelay, maxTimes);
    task_200ms = new Task(func_200ms, msDelay, maxTimes);
}

void loop() {
    task_100ms->update();
    task_200ms->update();
}
