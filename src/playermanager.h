#include "controllers.h"
#include <time.h>

class PlayerManager {

private:

	core::list<Character *> characters;

	Scene * scene;

	btVector3 spawnPoints[5];

public:
	
	PlayerManager(Scene * sceneIn);

	Character * addCharacter(btVector3 * position);
	
	//void update(f32 delta);

};