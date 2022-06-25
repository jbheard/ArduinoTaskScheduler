#include "task.h"

// Define some functions to schedule
void func_1s() {
    Serial.println("in func_1s");
    delay( Milliseconds::fromSeconds(1) );
}
void func_2s() {
    Serial.println("-in func_2s");
    delay( Milliseconds::fromSeconds(2) );
}
void func_3s() {
    Serial.println("--in func_3s");
    delay( Milliseconds::fromSeconds(3) );
}
void func_4s() {
    Serial.println("---in func_4s");
    delay( Milliseconds::fromSeconds(4) );
}


TaskScheduler scheduler;

void setup() {
    Serial.begin(115200);

	long msDelay = 500;
    long maxTimes = 2;

	Task *t;
    t = new Task(func_1s, msDelay, maxTimes);
    scheduler.addTask(t);
    t = new Task(func_2s, msDelay, maxTimes);
    scheduler.addTask(t);
    t = new Task(func_3s, msDelay, maxTimes);
    scheduler.addTask(t);
    t = new Task(func_4s, msDelay, maxTimes);
    scheduler.addTask(t);
}

void loop() {
    scheduler.update();
	if(scheduler.isDone()) {
		Serial.println("Finished all tasks!");
		while(1);
	}
}
