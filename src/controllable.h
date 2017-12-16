#pragma once

#include "weapons/weapons.h"

using namespace irr;

class Controllable{
public:

	irr::core::vector3df localForward;
	btVector3 trajectory;

	gui::IGUIEnvironment* gui;

	scene::ISceneNode * node;

	//btRigidBody * rigidBody;

	Entity * body;

	scene::ICameraSceneNode * cam;

	bool firing;	

	virtual void jump() = 0;

	virtual void zoom() = 0;

	//virtual void rotate(

	virtual void update(f32 delta, bool w, bool a, bool s, bool d, bool shift, f32 * Theta, f32 * Phi) = 0;

	//virtual bool align() = 0;

	virtual void activate(scene::ICameraSceneNode * camIn, gui::IGUIEnvironment* guiIn, Controllable * character) = 0;

	virtual void deactivate() = 0;

	//To replace the previous 2 functions
	virtual Controllable * changeView(Controllable * newVessel) = 0;

	virtual void reload() = 0;

	virtual void crouch(bool activate) = 0;

	virtual void switchWep() = 0;

	virtual void roll() = 0;

	virtual void destroy() = 0;

	virtual Controllable * use() = 0;
};