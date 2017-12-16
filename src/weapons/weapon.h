#pragma once
#include "../scene.h"

class Weapon {
public:

	gui::IGUIImage * reticule;

	struct gunstats {
		int ammo; //Max ammo per magazine
		int magazines; //Max amount of surplus ammo
		int rate; //Firing rate in miliseconds
		int life; //lifetime of projectile in miliseconds 		
		int accuracy; //Accuracy in integers (1 is most accurate)
		bool zoom; //Zoom enabled
		bool automatic; //auto fire enabled
		f32 speed; //projectile speed
		f32 zoomFov; //field of view of zoomed mode in radians
		f32 damage; //Damage of each projectile in hp points (players have 100)
	}stats;

	//ITexture * projectileTexture;

	gui::IGUIEnvironment* gui;

	int currentAmmo;
	//for auto fire
	int nextFire;

	IAnimatedMeshSceneNode* model;
	IAnimatedMeshSceneNode* worldModel;

	Scene * scene;

	/*Weapon(core::vector3df Position, ISceneManager *irrSceneIn, btDiscreteDynamicsWorld *WorldIn) {
		irrScene = irrSceneIn;
		World = WorldIn;
	}

	Weapon(ISceneManager *irrSceneIn, btDiscreteDynamicsWorld *WorldIn) {
		irrScene = irrSceneIn;
		World = WorldIn;

		loadModel();

		//model = getModel();
		/*model->setMaterialFlag(EMF_LIGHTING, true);
		model->setMaterialType(EMT_SOLID_2_LAYER );
		model->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);*//*
	}*/

	virtual void zoom(scene::ICameraSceneNode * cam) = 0;

	virtual void activate(scene::IAnimatedMeshSceneNode * playerModel) {
		//https://www.youtube.com/watch?v=2CewOjHHwR4

		worldModel->setParent(playerModel->getJointNode("hand.R"));
		worldModel->setPosition(core::vector3df(0,0,0));
		worldModel->setRotation(core::vector3df(0,0,0));
	}

	virtual void reload() = 0;

	virtual IAnimatedMeshSceneNode* getModel(const char* path) {
		//IAnimatedMesh* mesh = scene->getMesh(path);
		//IAnimatedMeshSceneNode* model = scene->getSceneManager()->addAnimatedMeshSceneNode( mesh );
		
		return scene->getMeshNode(path);
	}

	//virtual bool fire(scene::ISceneNode * view) = 0;
	virtual bool fire(scene::ICameraSceneNode * view) = 0;

	virtual void pickUp() = 0;

	//Need better name for this function
	virtual void view(scene::ICameraSceneNode * camIn, gui::IGUIEnvironment* gui) = 0;

	void endView(scene::ICameraSceneNode * cam, gui::IGUIEnvironment* gui) {
		model->remove();
		reticule->remove();
	}

	virtual void drop() = 0;

	virtual void deactivate(scene::IAnimatedMeshSceneNode * playerModel) {
		//if (model) {
			worldModel->setParent( scene->getRoot() );
			//worldModel->remove();
	}

	//virtual void storeTemp() = 0;

	virtual void parentCam(ICameraSceneNode * cam) {
		if (model)
			model->setParent(cam);
	}

	virtual void update(scene::ICameraSceneNode * cam, f32 rotationSpeed) {
		//model->setPosition(cam->getPosition());

		//model->setRotation(cam->getRotation());
		//model->setRotation(core::vector3df(model->getRotation().X, -rotationSpeed, model->getRotation().Z));

		/*if (model->getRotation().Y != cam->getRotation().Y) {
			if (model->getRotation().Y > cam->getRotation().Y) {
				model->setRotation(core::vector3df(model->getRotation().X, model->getRotation().Y - 5, model->getRotation().Z));
			} else {
				model->setRotation(core::vector3df(model->getRotation().X, model->getRotation().Y + 5, model->getRotation().Z));
			}
		}*/
	}

	virtual void destroy() = 0;
};
