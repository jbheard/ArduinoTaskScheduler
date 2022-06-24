#include "Arduino.h"
#include "repeat.h"

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

/***** Repeat class *****/

Repeat::Repeat( void (*func)(), long delayMs, int maxTimes) {
	this->func = func;
	this->delayMs = delayMs;
	this->maxTimes = maxTimes;
	times = (maxTimes > 0) ? 0 : -1;
	functionTimeTotal = 0;
	functionCalls = 0;
	functionTimeAverage = 0;
}

long Repeat::getFunctionTimeAverage() {
	return functionTimeAverage;
}

long Repeat::getDelayMs() {
	return delayMs;
}

void Repeat::update() {
	if(times >= maxTimes && maxTimes > 0) return; // Nothing to do
	if(delayMs > 0) {
		if(timer.getDuration() < delayMs)
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

/***** LinkedRepeatNode class *****/
// Used internally for RepeatScheduler only

RepeatScheduler::LinkedRepeatNode::LinkedRepeatNode(Repeat *repeat) {
	this->repeat = repeat;
}

void RepeatScheduler::LinkedRepeatNode::setNext(LinkedRepeatNode *next) {
	this->next = next;
}

RepeatScheduler::LinkedRepeatNode* RepeatScheduler::LinkedRepeatNode::getNext() {
	return this->next;
}

Repeat* RepeatScheduler::LinkedRepeatNode::getRepeat() {
	return repeat;
}

long RepeatScheduler::LinkedRepeatNode::getPriority() {
	// Leftover time after function has run
	return repeat->getFunctionTimeAverage() - repeat->getDelayMs();
}

/**********************************/

/***** RepeatScheduler class *****/

RepeatScheduler::RepeatScheduler() {
	length = 0;
	head = NULL;
}

void RepeatScheduler::addRepeat(Repeat *repeat) {
	LinkedRepeatNode *oldHead = head;
	this->head = new LinkedRepeatNode(repeat);
	head->setNext(oldHead);
	length ++;
}

void RepeatScheduler::update() {
	LinkedRepeatNode *last = NULL, *node = this->head, *next;
	while(node) {
		next = node->getNext();
		// Swap current with next if next has lower priority
		// Note: priority is time left after function finishes but before next execution,
		// 	     essentially we want to maximize leftover time
		if(next != NULL && next->getPriority() < node->getPriority()) {
			LinkedRepeatNode *nextnext = next->getNext();
			if(last) {
				last->setNext(next);
				next->setNext(node);
				node->setNext(nextnext);
			} else {
				this->head = next;
				next->setNext(node);
				node->setNext(nextnext);
			}
			node = next;
		}
		node->getRepeat()->update();
		last = node;
		node = next;
	}
}

/*********************************/
