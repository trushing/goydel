#pragma once
#include "eventnode.h"

class EventQueue {
private:

	EventNode * head;

public:
	EventQueue();

	void enqueue(void* (*function)(const void*,const void*), int time);

	void update(int time);
};