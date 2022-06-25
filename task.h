#ifndef TASK_H
#define TASK_H

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

class Task {
public:
    Task( void (*func)(), long delayMs = -1, int maxTimes = 0);

	// Checks time since last run & calls the function if delayMs has been hit
	// Runs the function every time if delayMs == -1
    void update();

	// Get the average execution time of the function
	long getFunctionTimeAverage();

	// How many times has the underlying function been called?
	long numCalls();

	// Get the delay set in the constructor
	long getDelayMs();

	// Check if the function has been run maxTimes, if maxTimes <= 0 this 
	// will always be true
	bool isDone();

private:
    void (*func)();
    Timer timer;
    long delayMs;
    int maxTimes;
	unsigned long functionTimeTotal;
	long functionCalls;
	long functionTimeAverage;
};


class TaskScheduler {
public:
	TaskScheduler();

	// Add a new task to the scheduler
	void addTask(Task *task);
	
	// Call task.update() for all (not finished) tasks
	void update();
	
	// True if all tasks have finished running, False otherwise
	// Will always be false if >= 1 task has maxTimes <= 0 (see Task::isDone() above)
	bool isDone();

private:
	class LinkedTaskNode {
	public:
		LinkedTaskNode(Task *task);
		void setNext(LinkedTaskNode *next);
		LinkedTaskNode *getNext();
		Task *getTask();
	private:
		Task *task;
		LinkedTaskNode *next;
	};

	int length;
	LinkedTaskNode *head;
};

#endif // TASK_H

