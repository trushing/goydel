#include "eventnode.h"

EventNode::EventNode(void* (*functionIn)(const void*,const void*), int timeIn) {

	function = functionIn;
	time = timeIn;

	tail = NULL;

}

void EventNode::enqueue(EventNode * node) {
	if (tail != NULL) {
			if(tail->LaterThan(node->time)) {
				node->setTail(tail);
				tail = node;
			} else {
				tail->enqueue(node);
			}
	} else {
		tail = node;
	}
}

void EventNode::setTail(EventNode * newTail) {
	tail = newTail;
}

bool EventNode::LaterThan(int time2) {
	return time >= time2;
}

EventNode * EventNode::execute() {
	//function();
	return tail;
}