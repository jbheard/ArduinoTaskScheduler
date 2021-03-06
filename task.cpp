#include "Arduino.h"
#include "task.h"

/***** Timer class *****/

Timer::Timer() {
	startTime = millis();
}

void Timer::reset() {
	startTime = millis();
}

unsigned long Timer::getDuration() {
	unsigned long _time = millis();
	if(_time < startTime) {
		// 4,294,967,295 is unsigned long max value
		return (4294967295L - startTime) + _time + 1; // Add 1 to account for overflow
	}
	return _time - startTime;
}

/***********************/

/***** Task class *****/

Task::Task( void (*func)(), long delayMs, int maxTimes) {
	this->func = func;
	this->delayMs = delayMs;
	this->maxTimes = maxTimes;
	functionTimeTotal = 0;
	functionCalls = 0;
	functionTimeAverage = 0;
}

long Task::getFunctionTimeAverage() {
	return functionTimeAverage;
}

long Task::numCalls() {
	return functionCalls;
}

long Task::getDelayMs() {
	return delayMs;
}

bool Task::isDone() {
	return functionCalls >= maxTimes && maxTimes > 0;
}

void Task::update() {
	if(functionCalls >= maxTimes && maxTimes > 0) return; // Nothing to do
	if(delayMs > 0) {
		if(timer.getDuration() < (unsigned long)delayMs)
			return;
		else
			timer.reset();
	}

	Timer tempTimer;
	func();
	unsigned long _time = tempTimer.getDuration();

	functionCalls ++;
	functionTimeTotal += _time;
	functionTimeAverage = functionTimeTotal / functionCalls;
}

/************************/

/***** LinkedTaskNode class *****/
// Used internally for TaskScheduler only

TaskScheduler::LinkedTaskNode::LinkedTaskNode(Task *task) {
	this->task = task;
	this->next = NULL;
}

void TaskScheduler::LinkedTaskNode::setNext(LinkedTaskNode *next) {
	this->next = next;
}

TaskScheduler::LinkedTaskNode* TaskScheduler::LinkedTaskNode::getNext() {
	return this->next;
}

Task* TaskScheduler::LinkedTaskNode::getTask() {
	return task;
}

/**********************************/

/***** TaskScheduler class *****/

TaskScheduler::TaskScheduler() {
	head = NULL;
}

void TaskScheduler::addTask(Task *task) {
	LinkedTaskNode *newNode = new LinkedTaskNode(task);

	if(this->head == NULL) {
		this->head = newNode;
		return;
	}

	LinkedTaskNode *node = this->head;
	while(node->getNext()) {
		node = node->getNext();
	}
	node->setNext(newNode);
}

bool TaskScheduler::isDone() {
	return this->head == NULL;
}

void TaskScheduler::update() {
	LinkedTaskNode *prev = NULL, *node = this->head;
	while(node) {
		// Remove any completed nodes from linked list
		if(node->getTask()->isDone()) {
			LinkedTaskNode *next = node->getNext();
			if(prev) {
				prev->setNext(next);
			} else {
				this->head = next;
			}
			delete node;
			node = next;
			Serial.println("Deleted task");
			continue;
		}

		node->getTask()->update();

		prev = node;
		node = node->getNext();
	}
}

/*********************************/
