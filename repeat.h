#ifndef REPEAT_H
#define REPEAT_H

#include "Arduino.h"

class Milliseconds {
public:
	// Note: should keep this one relatively low, since 2^32 is only 49 days, we could quickly overflow
	static unsigned long fromDays(long days) {
		return (unsigned long)days * 3600 * 24000;
	}

	static unsigned long fromHours(long hours) {
		return (unsigned long)hours * 3600000;
	}

	static unsigned long fromMinutes(long minutes) {
		return (unsigned long)minutes * 60000;
	}

	static unsigned long fromSeconds(long seconds) {
		return (unsigned long)seconds * 1000;
	}

	static long toSeconds(unsigned long ms) {
		return ms / 1000;
	}

	static long toMinutes(unsigned long ms) {
		return ms / 60000;
	}

	static long toHours(unsigned long ms) {
		return ms / 3600000;
	}

	static long toDays(unsigned long ms) {
		return ms / (24000 * 3600);
	}
};

class Timer {
public:
    Timer();
    void reset();
    unsigned long getDuration();

private:
    unsigned long startTime;
};

class Repeat {
public:
    Repeat( void (*func)(), long delayMs = -1, int maxTimes = 0);
    void update();
	long getFunctionTimeAverage();
	long getDelayMs();

private:
    void (*func)();
    Timer timer;
    long delayMs;
    int maxTimes;
    int times;
	unsigned long functionTimeTotal;
	long functionCalls;
	long functionTimeAverage;
};

class RepeatScheduler {
public:
	RepeatScheduler();
	void addRepeat(Repeat *repeat);
	void update();

private:
	class LinkedRepeatNode {
	public:
		LinkedRepeatNode(Repeat *repeat) {
			this->repeat = repeat;
		}
		
		void setNext(LinkedRepeatNode *next) {
			this->next = next;
		}
		
		LinkedRepeatNode *getNext() {
			return this->next;
		}
		
		Repeat *getRepeat() {
			return repeat;
		}
		
		long getPriority() {
			// Leftover time after function has run
			return repeat->getFunctionTimeAverage() - repeat->getDelayMs();
		}

	private:
		Repeat *repeat;
		LinkedRepeatNode *next;
	};

	int length;
	LinkedRepeatNode *head;
};

#endif // REPEAT_H
