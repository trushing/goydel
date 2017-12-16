#pragma once
#include "controllers.h"

#include "controls.h"

LocalController::LocalController(Scene * sceneIn, gui::IGUIEnvironment* guiIn) {

	Radius = 20.f;
    Theta  = 180.f; // degrees
    Phi    = 90.f; // degrees

	f32 LinearVelocity = 5.f;
	f32 AngularVelocity = 5.f;

	editMode = false;

	gui = guiIn;
	scene = sceneIn;

	switchCool = 0;

	vessel = NULL;

	ScreenCenter.X = scene->irrDriver->getScreenSize().Width / 2;
	ScreenCenter.Y = scene->irrDriver->getScreenSize().Height / 2;

	//Spectator * spectator = new Spectator(scene);
	//vessel = spectator;

	//control(spectator);

	//Player * player = new Player(scene, &btVector3(0,20,0));
	//control(player);

    //warning: converting ‘false’ to pointer type ‘Weapon*’ [-Wconversion-null]
        //w_active = false;
    //w_active = NULL;

    //terrain = terrainIn;

    //editMode = false;
    MouseState.right = false;
    MouseState.left = false;
    //camMod = new SinWave(1.0f,1.0f,1.0f);

    //Establish pointers to Irrlicht components

    //playerNode = Scene->addEmptySceneNode();

    //Create player camera
    //cam = irrScene->addCameraSceneNodeFPS(node, 30 , 0, -1, keyMap, 8, true);
	cam = scene->getSceneManager()->addCameraSceneNodeFPS(0, 100.0f,0);
	//cam = scene->getSceneManager()->addCameraSceneNode();
    cam->bindTargetAndRotation(true);
    cam->setNearValue(1);
    zoomOut();
    cam->setFarValue(10000);

    for (int i = 0; i < KEY_KEY_CODES_COUNT; i++)
    {
        KeyIsDown[i] = false;
    }

    //if (isOnGround()) {
        jumpPrimed = true;
    //} else {
    //	jumpPrimed = false;
    //}

    character = NULL;
}

void LocalController::ToggleEdit() {

	printf("toggle edit");

    if (editMode) {

        //Switch to fighting mode

		//edit_temp->destroy();

		control(edit_temp);

		edit_temp = NULL;

        //*interaction() = *fighterInteraction();

        //weapon->setVisible(true);

        //giveWep(editor->getTemp());

        //playerModel->setVisible(true);
        //bodyModel->setVisible(true);

        //RigidBody->setGravity(scene->getGravity());

        //RigidBody->setDamping(0,0);

        //terrain->getMaterial(0).setFlag(irr::video::EMF_WIREFRAME, false);

    } else {

		btVector3 * position = new btVector3(vessel->body->physics->getCenterOfMassPosition());

		//position = *vessel->rigidBody->getCenterOfMassTransform().getOrigin();

		Spectator * spectator = new Spectator(scene, position);

		edit_temp = vessel;

		control(spectator);

        //Switch to edit mode

        //editor->storeTemp(w_active);

        //giveWep(editor);

        //weapon->setVisible(false);
        //playerModel->setVisible(false);
        //bodyModel->setVisible(false);

        //RigidBody->setGravity(btVector3(0,0,0));
        //RigidBody->setDamping(0.9f,0.9f);

        //terrain->getMaterial(0).setFlag(irr::video::EMF_WIREFRAME, true);
    }

    editMode = !editMode;
}

void LocalController::control(Controllable * vesselIn) {
	if (vessel != NULL)
			vessel->deactivate();


	vesselIn->activate(cam, gui, vessel);
	vessel = vesselIn;

	printf("controlled");
}

//Event capturing function
bool LocalController::OnEvent(const SEvent& event) {

    //interaction();

    switch (event.EventType)
    {
    case EET_KEY_INPUT_EVENT:
        {

			if (event.KeyInput.Key == KEY_PERIOD && !KeyIsDown[KEY_PERIOD]) {
				scene->entities->removeLast();
			}

			if (event.KeyInput.Key == KEY_COMMA && !KeyIsDown[KEY_COMMA]) {
				scene->entities->removeFirst();
			}

			if (event.KeyInput.Key == KEY_KEY_V && !KeyIsDown[KEY_KEY_V]) {
                //Switch to edit mode

                ToggleEdit();
            }

			if (event.KeyInput.Key == KEY_LCONTROL) {
				if (!KeyIsDown[KEY_LCONTROL]) {

					vessel->crouch(true);
				} else {
					vessel->crouch(false);
				}

            }
			if(controls.keyremap)
				input.remap(controls.foot.toggle_edit, event.KeyInput.Key);
			controls.keyremap = false;

			if(event.KeyInput.Key == KEY_KEY_P)
				controls.keyremap = true;

			if(event.KeyInput.Key == KEY_KEY_E && !KeyIsDown[KEY_KEY_E]) {

				Controllable * controllable = vessel->use();

				if (controllable != NULL) {
					character = vessel;
					control(controllable);
				} else {
					if (character != NULL) {
						control(character);
						character = NULL;
					}
				}
			}

			if(event.KeyInput.Key == KEY_KEY_R)
				vessel->reload();

			if(event.KeyInput.Key == KEY_LMENU) {
				vessel->roll();
			}

			if(event.KeyInput.Key == KEY_KEY_Q) {

				if (scene->getTime() >= switchCool) {
					switchCool = scene->getTime() + 500;
					vessel->switchWep();
				}

			}

            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
            //printf("pressed\n");



			if (event.KeyInput.Key == KEY_SPACE) {//controls.foot.jump) {
                //if (event.KeyInput.PressedDown) {
                //    if (jumpPrimed == true && !editMode/* && isOnGround()*/) {
                //        //printf("//space pressed//");
				//		printf("jump");
                //        jumpPrimed = false;
                //
                        //printf("ayy\n");
                        vessel->jump();
               //     }
              //  } else {
               //     //printf("//space lifted//");
               //     jumpPrimed = true;
              //  }
            }

            if (event.KeyInput.Key == KEY_LBUTTON) {
                //fire();
            }

            /*if (event.KeyInput.Key == KEY_ESCAPE) {
                *quit = true;
            }*/

            break;
        }

    case EET_MOUSE_INPUT_EVENT:
        {
            //
            //updateTrajectory();
            //updateWeapon();

			if (event.MouseInput.Event == EMIE_MOUSE_MOVED) {

				//printf("\n%i", event.MouseInput.X);

				if (event.MouseInput.X != ScreenCenter.X || event.MouseInput.Y != ScreenCenter.Y) {
					scene->irrDevice->getCursorControl()->setPosition(0.5f,0.5f);

					CursorDiff.X = event.MouseInput.X - ScreenCenter.X;
					CursorDiff.Y = event.MouseInput.Y - ScreenCenter.Y;
				}

				Cursor.X = event.MouseInput.X;
                Cursor.Y = event.MouseInput.Y;
			}

            if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
            {

                //printf("%f\n", trajectory.z());

                //node->setPosition(core::vector3df(0, node->getPosition().Y + 0.01f, 0));
                //fire();
                //printf("firing");
				if (!MouseState.left) {
					MouseState.left = true;
					vessel->firing = true;
				}

                //core::vector3df targetdir = cam->getTarget() - cam->getPosition();

                //vessel->fire(targetdir, cam);

            }

			if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
            {
                vessel->zoom();
            }

			if (event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
            {
                zoomOut();
            }

            if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
            {
                MouseState.left = false;
				vessel->firing = false;
            }

			/*

            if (event.MouseInput.Event == EMIE_MOUSE_MOVED)
            {

               // playerNode->setRotation(core::vector3df(0, cam->getRotation().Y ,0));

                //Xdirection = Xdirection + (event.MouseInput.X - 400);

                //cam->setRotation(core::vector3df(0, Xdirection, 0));
                //RigidBody->set

                //printf("%d\n",Xdirection);

                double zero = 0;

                if (event.MouseInput.X != 400) {
                    //printf("resetting mouse");
                    //irrDevice->getCursorControl()->setPosition(0.5f,0.5f);
                }
            }*/

			if (event.MouseInput.Event == EMIE_MOUSE_WHEEL) {
				if (scene->getTime() >= switchCool) {

					zoomOut();

					switchCool = scene->getTime() + 500;
					vessel->switchWep();
				}
			}
        }
    default:
        {
        break;
        }
    }

    return false;
}

//Tests a key to see if it's pressed down
bool LocalController::isKeyDown(EKEY_CODE keyCode) const
{
    return KeyIsDown[keyCode];
}

bool LocalController::update(f32 delta) {

	if(CursorDiff.X != 0)
    {
        Theta += ((AngularVelocity * (delta / 1000)) + CursorDiff.X / 2);
    }
    if(CursorDiff.Y != 0)
    {
        Phi += ((AngularVelocity * (delta / 1000)) + CursorDiff.Y / 2);
    }

	//OldCursor = Cursor;

	CursorDiff.X = 0;

	CursorDiff.Y = 0;

//if(isKeyDown(KEY_KEY_W) || isKeyDown(KEY_KEY_S) || isKeyDown(KEY_KEY_A) || isKeyDown(KEY_KEY_D)){
	if (vessel->body->node == NULL) {

		vessel->destroy();
		vessel = NULL;

		return false;

	} else {

		vessel->update(delta, isKeyDown(KEY_KEY_W), isKeyDown(KEY_KEY_A), isKeyDown(KEY_KEY_S), isKeyDown(KEY_KEY_D), isKeyDown(KEY_LSHIFT), &Theta, &Phi );

		return true;

	}

	//scene->irrDevice->getCursorControl()->setPosition(0.5f,0.5f);

    //node->setPosition(core::vector3df(0, node->getPosition().Y + 0.01f, 0));

    //cam->updateAbsolutePosition();
    //if(MouseState.left)
    //{
    //    core::vector3df targetdir = cam->getTarget() - cam->getPosition();
       // w_active->fire(&targetdir);
    //}



	//w_active->update(cam);

    //cam->setTarget(vector);
}

void LocalController::zoomOut() {
	cam->setFOV(1.57f);
}

NetworkController::NetworkController() {

}

void NetworkController::control(Controllable * vesselIn) {

}

bool NetworkController::update(f32 Delta) {
	return true;
}

AIController::AIController(Scene * scene) {
	vessel = NULL;

	cam = scene->getSceneManager()->addCameraSceneNode();
	cam->bindTargetAndRotation(true);


}

void AIController::control(Controllable * vesselIn) {

		//printf("\ncontrolling");

	if (vessel != NULL)
		vessel->deactivate();

	vessel = vesselIn;

	vessel->activate(cam, NULL, NULL);

	//vessel->firing = true;
}

bool AIController::update(f32 delta) {

	//View vector in degrees (phi is vertical, theta is horizontal)
	f32 phi = 90;
	f32 theta = 0;

	//vessel->reload();



	if (vessel->body->node == NULL){

		vessel->destroy();

		vessel = NULL;

		return false;

	} else {

		theta += 0.01;

		vessel->update(delta, false, false, false, false, false, &theta, &phi);

		return true;
	}

}
