#include "includes.h"

using namespace irr;

class Destructible {

public:
	
	f32 health;

	virtual void destroy() = 0;

	virtual void damage(f32 amount, core::vector3df * origin) = 0;
	
	/*void damage(f32 amount) {

		//destroy();

		//printf("%f\n", amount);
		//printf("%i\n", health);

		if (health > amount) {
			health -= amount;
		} else {
			destroy();
		}
	}*/


};