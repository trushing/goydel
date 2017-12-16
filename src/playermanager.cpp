#include "playermanager.h"
	
PlayerManager::PlayerManager(Scene * sceneIn) {

	scene = sceneIn;

	spawnPoints[0] = btVector3(100,10,100);
	spawnPoints[1] = btVector3(-100,10,100);
	spawnPoints[2] = btVector3(0,10,100);
	spawnPoints[3] = btVector3(-100,10,-100);
	spawnPoints[4] = btVector3(100,10,0);
}

Character * PlayerManager::addCharacter(btVector3 * position) {

	if (position == NULL) {

		Character * character = new Character(scene, &spawnPoints[(int) (rand()%4)]);

		characters.push_back(character);

		return character;

	}else {
		srand(time(NULL));

		Character * character = new Character(scene, position);

		characters.push_back(character);

		return character;
	}
		
}
	
/*
void PlayerManager::update(f32 delta) {
		
	core::list<Character *>::Iterator Iterator;
	// Relay the object's orientation to irrlicht
	for(Iterator = characters.begin(); Iterator != characters.end();) {

		//printf("player", Iterator);

		if (!(*Iterator)->align()) {

			Iterator = characters.erase(Iterator);
				
			//scene->addEvent(addCharacter, 5);

			addCharacter(NULL);
		} else {
			Iterator++;
		}
	}
	//printf("\n");
}*/