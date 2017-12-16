#pragma once
#include "client.h"

Client::Client() {

    irrDevice = createDevice(video::EDT_OPENGL, core::dimension2du(800, 600), 32 ,false, false, true);

	video::IVideoDriver * irrDriver = irrDevice->getVideoDriver();

	gui::IGUIEnvironment* gui = irrDevice->getGUIEnvironment();	

	u32 screenheight = irrDriver->getScreenSize().Height;
	u32 screenwidth = irrDriver->getScreenSize().Width;

    scene = new Scene(irrDevice);

    irrTimer = irrDevice->getTimer();

	//Test npc
	npc = new AIController(scene);

	//Add the player
    controller = new LocalController(scene, gui);
    irrDevice->setEventReceiver(controller);

	playerManager = new PlayerManager(scene);    

    // Loop delta timer to check how many miliseconds changes between each frame
    u32 TimeStamp = irrTimer->getTime(), DeltaTime = 0;	
	u32 TimeStamp2 = 0;

    //Used to end game loop
    done = false;	

	core::matrix4 viewProj;

	//irrTimer->setSpeed(2);
	btVector3 vec1 = btVector3(0,60,10);
	btVector3 vec2 = btVector3(0,100,100);	

	Car * car = new Car(scene, &btVector3(100,100,200));

	Fighter * fighter = new Fighter(scene, &btVector3(-200,200,200));

	//controller->control(playerManager->addCharacter(&vec1));

	Character * character = playerManager->addCharacter(&vec1);

	controller->control(character);
	//controller->control(car);
	//controller->control(fighter);

	npc->control(playerManager->addCharacter(&vec2));

	//npc->control(car);

    //Game loop ---------------------------------------------------------------------------------------- Updates every frame
    while(!done) {
		
		DeltaTime = irrTimer->getTime() - TimeStamp;
        TimeStamp = irrTimer->getTime();

        if (!controller->update(DeltaTime)) {
			controller->control(playerManager->addCharacter(NULL));
		}

		if (!npc->update(DeltaTime)) {
			npc->control(playerManager->addCharacter(NULL));
		}	
				
		scene->update(DeltaTime);

        if (irrDevice->isWindowActive()) {

			//TimeStamp2 = irrTimer->getTime();

			if (irrDriver->beginScene(true, true, video::SColor(0x0))) {	

				scene->getSceneManager()->drawAll();
				
				gui->drawAll();

				irrDriver->endScene();

			}

			//playerManager->update(DeltaTime);

			

			//printf("updating effect");

			

			

			//printf("%u\n", irrTimer->getTime() - TimeStamp2



        }

		 if (controller->isKeyDown(KEY_ESCAPE)) {
            printf("done\n");
            done = true;
        }
		 
		irrDevice->run();

        //player.updateWeapon();			
    }

    //When the game loop is interrupted, game world closes down.

    printf("quitting\n");
    delete controller;

    irrDevice->drop();

    //getc(stdin);
}
