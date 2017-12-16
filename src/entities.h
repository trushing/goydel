#include "weapons/projectile.h"

class Entities {

public:

	Entity * head;

	Entities(Entity * ground) {
		head = ground;
	}

	void add(Entity * entity) {

		if (head->next != NULL)
			head->next->prev = entity;

		entity->prev = head;
		entity->next = head->next;

		head->next = entity;
	}


	//These methods are for testing purposes

	void removeLast() {

		printf("\n--------------------------");

		printf("\nremoving last entity");

		Entity * entity;		

		int length = 0;

		entity = head;

		printf("\naddress of head: %p", head);

		while (entity->next != NULL) {
			entity = entity->next;
			length += 1;
			printf("\naddress of entity: %p", entity);
			printf(", address of prev :%p", entity->prev);
			printf(", address of next :%p", entity->next);
		}

		printf("\nlength of list before deletion: %i", length); 

		if (entity->prev != NULL) {
			printf("\nlast entity has prev");
		} else {
			printf("\nlast entity has no prev");
		}

		if (entity != head) {
			entity->remove();
		}
		
		length = 0;

		entity = head;

		while (entity->next != NULL) {
			entity = entity->next;
			length += 1;
		}

		printf("\nlength of list after deletion: %i", length); 

		printf("\n--------------------------");
	}

	void removeFirst() {
		head->next->remove();
	}

	/*void add(Entity * entity) {

		if (head == NULL) {
			head = entity;
		}else {

			entity->next = head;
			head->prev = entity;

			head = entity;
		
		}
	}

	void remove(Entity * entity) {

		if (entity == head) {
			printf("removing the head");
			head = head->next;
		}

		entity->remove();
		
	}*/
};