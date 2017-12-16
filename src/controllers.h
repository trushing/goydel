#pragma once
#include "controller.h"

using namespace irr;

class LocalController : public IEventReceiver, public Controller
{
private: 

	f32 Radius;
	f32 Theta; // degrees
	f32 Phi; // degrees

	f32 LinearVelocity;
	f32 AngularVelocity;

	Controllable * edit_temp;
	Controllable * character;

	//The thing the player is controlling
    struct Mstate
    {
        bool right;
        bool left;
        
    }MouseState;
	
	f32 switchCool; //stores a time for cooling weapon switch

	bool editMode;

	bool KeyIsDown[KEY_KEY_CODES_COUNT];

	bool jumpPrimed;

	gui::IGUIEnvironment* gui;

	core::position2d<f32> Cursor;
	core::position2d<f32> OldCursor;
	core::position2d<f32> CursorDiff;

	core::position2d<f32> ScreenCenter;
	
public: 

	void control(Controllable * vesselIn); 

	void ToggleEdit();

	LocalController(Scene * scene, gui::IGUIEnvironment* guiIn);
	//Update camera positing to emulate head bobbing
	//void updateCam();
	//Applys trajectory to player movement
	//void updateMovement(float delta);
	//Event capturing function
	bool OnEvent(const SEvent& event);
	//void interaction();
	//void fighterInteraction();
	//void editorInteraction();
	//void updateWeapon();
	//Tests a key to see if it's pressed down
	bool isKeyDown(EKEY_CODE keyCode) const;

	void zoomOut();

	//Checks if player is on the ground
	//bool isOnGround();
	//Sets velocity of player from a bullet vector
	//void setVelocity(btVector3 &velocity);
	//Returns player velocity as a vector
	//btVector3 getVelocity();
	//Probably not needed
	//void rotateToView(core::vector3df * vector);
	bool update(f32 delta);
	//void setVehicle(Vehicle *vehicleIn) {
	//	vehicle = vehicleIn;
	//}

	//core::vector3df getPosition();
	//float distanceTo(ISceneNode *node);
	//void giveWep( Weapon *weapon );
};

//Controller for networked players
class NetworkController : public Controller {

public:
	NetworkController();

	void control(Controllable * vesselIn);

	bool update(f32 Delta);
};

//Controller for AI
class AIController : public Controller {

public:

	AIController(Scene * scene);

	void control(Controllable * vesselIn);

	bool update(f32 delta);
};
