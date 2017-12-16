#include "entities.h"

class EventNode{
private:

	void * (*function)(const void*,const void*);

	EventNode * tail;

public:

	int time;

	EventNode(void* (*functionIn)(const void*,const void*), int timeIn);

	void enqueue(EventNode * node);

	void setTail(EventNode * newTail);

	bool LaterThan(int time2);

	EventNode * execute();
};