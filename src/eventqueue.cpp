#include "eventqueue.h"

EventQueue::EventQueue() {
	head = NULL;
}

void EventQueue::enqueue(void* (*function)(const void*,const void*), int time) {

	EventNode * newEvent = new EventNode(function, time);

	if (head) {
		head = newEvent;
	} else {
		head->enqueue(newEvent);
	}		
}

void EventQueue::update(int time) {
	if (head != NULL) {
		if (head->LaterThan(time)) {
			head = head->execute();
		}
	}
}