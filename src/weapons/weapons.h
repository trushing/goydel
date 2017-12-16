#pragma once
#include "weapon.h"

class Rifle : public Weapon {

private:

	int currentAccuracy;

	int lastFire;

public:	

	Rifle(Scene * sceneIn, core::vector3df location) {	

		setStats();

		scene = sceneIn;		

		worldModel = getModel("../Models/riflemodel.b3d");
		worldModel->setScale(core::vector3df(1.0f,1.0f,1.0f));
		worldModel->setPosition(location);

		//preload
		scene->preloadMesh("../Models/rifle.b3d");

		model = NULL;
	}

	Rifle(Scene * sceneIn, scene::IAnimatedMeshSceneNode * playerModel) {

		setStats();

		scene = sceneIn;		

		worldModel = getModel("../Models/riflemodel.b3d");
		worldModel->setScale(core::vector3df(1.0f,1.0f,1.0f));

		//preload
		scene->preloadMesh("../Models/rifle.b3d");

		activate(playerModel);

		model = NULL;
	}

	void setStats() {
		//Would like to read these from a file for each weapon
		stats.ammo = 20;
		stats.magazines = 3;
		stats.rate = 70;
		stats.life = 5000;
		stats.zoom = true;
		stats.zoomFov = 0.5;
		stats.speed = 5;
		stats.automatic = true;
		stats.damage = 5;
		stats.accuracy = 1;

		currentAccuracy = stats.accuracy;

		//Set the amount of ammo for weapon
		currentAmmo = stats.ammo;		

		lastFire = 0;
		nextFire = 0;
	}

	void deactivate(scene::IAnimatedMeshSceneNode * playerModel) {
		worldModel->setParent(playerModel->getJointNode("chest"));
		worldModel->setRotation(core::vector3df(0,0,150));
		worldModel->setPosition(core::vector3df(0.5f,0.9f,-0.2f));
	}

	void reload() {
		printf("gun reloaded");
		currentAmmo = stats.ammo;
	}

	void view(scene::ICameraSceneNode * cam, gui::IGUIEnvironment* gui) {
		model = getModel("../Models/rifle.b3d");
		model->setScale(core::vector3df(1.0f,1.0f,1.0f));
		//model->setRotation(core::vector3df(0,180,0));
		//model->setPosition(core::vector3df(-0.5f,-9.5f,4.0f));

		model->setPosition(core::vector3df(0.0f,-0.3f,0.65f));

		model->setParent(cam);

		model->setLoopMode(false);

		model->setMaterialFlag(video::EMF_LIGHTING, true);
		model->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		
		//model->setDebugDataVisible( EDS_SKELETON );

		model->addShadowVolumeSceneNode();

		//if (gui != NULL) {
			reticule = scene->getReticule(gui, "../Textures/reticule.png"); 
		//}
	}	

	bool fire(scene::ICameraSceneNode * view) {

		//printf("%f\n", cam->getAbsolutePosition().Y);

		int timeGap = lastFire + stats.rate;

		if (scene->getTime() >= timeGap && currentAmmo > 0 ) {

			//printf("%i\n", currentAccuracy);

			//currentAccuracy -= (timeGap / 3);

			//currentAccuracy = std::max(currentAccuracy - (timeGap / 30), stats.accuracy);
			//currentAccuracy = std::max(currentAccuracy, stats.accuracy);

			lastFire = scene->getTime();
			
			core::vector3df trajectory = view->getTarget() - view->getPosition();

			trajectory.X += (rand() % currentAccuracy) - (currentAccuracy / 2);
			trajectory.Y += (rand() % currentAccuracy) - (currentAccuracy / 2);
			trajectory.Z += (rand() % currentAccuracy) - (currentAccuracy / 2);

			//currentAccuracy += timegap;

			//core::vector3df trajectory = view->getRotation();

			nextFire = scene->getTime() + stats.rate; //Firing rate in miliseconds

			currentAmmo -= 1;

			//printf("firing\n");

			IBillboardSceneNode * sprite = scene->getSceneManager()->addBillboardSceneNode(0,  core::dimension2d<f32>(20, 20));
			sprite->setMaterialFlag(video::EMF_LIGHTING, false);
			sprite->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			sprite->setMaterialTexture(0, scene->getTexture("../Textures/plasma.png"));

			trajectory.normalize();
			//printf("X %f, Y %f, Z %f\n",trajectory.X,trajectory.Y,trajectory.Z);

			//sprite->setPosition(core::vector3df(cam->getAbsolutePosition().X + trajectory->X * 15, cam->getAbsolutePosition().Y + trajectory->Y * 15, cam->getAbsolutePosition().Z + trajectory->Z * 15));		

			//btVector3 * position = new btVector3(cam->getAbsolutePosition().X + trajectory.X * 15, cam->getAbsolutePosition().Y + trajectory.Y * 15, cam->getAbsolutePosition().Z + trajectory.Z * 15);
			btVector3 * position = new btVector3(view->getAbsolutePosition().X + trajectory.X * 30, view->getAbsolutePosition().Y + trajectory.Y * 30, view->getAbsolutePosition().Z + trajectory.Z * 30);


			//btVector3 * position = new btVector3(cam->getAbsolutePosition().X + trajectory.X * 30, cam->getAbsolutePosition().Y + trajectory.Y * 30, cam->getAbsolutePosition().Z + trajectory.Z * 30);

			//sprite->setRotation(model->getRotation());
			
			scene->lightUp(sprite, video::SColorf(57, 181 , 74, 0));

			//Particle emitter

			ISceneManager * smgr = scene->getSceneManager();		

			//-----------------Trail

			scene::IParticleSystemSceneNode* trail = smgr->addParticleSystemSceneNode(false);

			//trail->setParticleSize(core::dimension2df(10, 10));

			scene::IParticleEmitter* emitter = trail->createBoxEmitter(
				core::aabbox3d<f32>(1,1,1,1,1,1), // emitter size
				core::vector3df(0.0f,0.00f,0.0f),   // initial direction
				10,1); 

			emitter->setMinStartSize(core::dimension2df(15,15));
			emitter->setMaxStartSize(core::dimension2df(15,15));

			emitter->setMaxStartColor(video::SColor(0,255,255,255));
			emitter->setMinStartColor(video::SColor(0,255,255,255));

			emitter->setMinLifeTime(100);
			emitter->setMaxLifeTime(100);

			trail->setEmitter(emitter); // this grabs the emitter
			//emitter->drop(); // so we can drop it here without deleting it

			//scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
			scene::IParticleAffector* shrink = trail->createScaleParticleAffector(core::dimension2df(-15,-15));

			//ps->addAffector(paf); // same goes for the affector
			trail->addAffector(shrink);
			//paf->drop();
			//shrink->drop();

			//ps->setPosition(core::vector3df(-70,60,40));
			//ps->setScale(core::vector3df(2,2,2));
			trail->setMaterialFlag(video::EMF_LIGHTING, false);
			trail->setMaterialFlag(video::EMF_ZWRITE_ENABLE, true);
			trail->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("../Textures/plasma.png"));
			trail->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

			trail->setParent(sprite);

			//--------------------Impact

			scene::IParticleSystemSceneNode* impact = smgr->addParticleSystemSceneNode(false);

			//impact->setParticlesAreGlobal(true);

			impact->setVisible(false);

			emitter = impact->createBoxEmitter(
				core::aabbox3d<f32>(1,1,1,1,1,1), // emitter size
				core::vector3df(0.0f,0.5f,0.0f));   // initial direction

			emitter->setMaxAngleDegrees(90);

			emitter->setMaxParticlesPerSecond(20);
			emitter->setMinParticlesPerSecond(20);

			emitter->setMinStartSize(core::dimension2df(10,10));
			emitter->setMaxStartSize(core::dimension2df(20,20));

			emitter->setMaxStartColor(video::SColor(0,255,255,255));
			emitter->setMinStartColor(video::SColor(0,255,255,255));

			emitter->setMinLifeTime(50);
			emitter->setMaxLifeTime(100);

			impact->setEmitter(emitter); // this grabs the emitter
			emitter->drop(); // so we can drop it here without deleting it

			impact->setMaterialFlag(video::EMF_LIGHTING, false);
			impact->setMaterialFlag(video::EMF_ZWRITE_ENABLE, true);
			impact->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("../Textures/plasma.png"));
			impact->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

			scene::IParticleAffector* grav = impact->createGravityAffector(core::vector3df(0,-5.0, 0));
			shrink = impact->createScaleParticleAffector(core::dimension2df(-15,-15));

			//ps->addAffector(paf); // same goes for the affector
			impact->addAffector(grav);
			//paf->drop();
			grav->drop();

			//ps->addAffector(paf); // same goes for the affector
			impact->addAffector(shrink);
			//paf->drop();
			shrink->drop();

			//---------------------emitters done
		
			// Projectile object contains a sprite and keeps track of when to destroy it
			scene->addProjectile(new Projectile(sprite, position, trajectory, scene->getTime() + stats.life, impact, stats.speed, stats.damage));
		}

		return stats.automatic;

	}

	void pickUp() {
		printf("pick up");
	}

	void drop() {

		scene->addEntity("../Models/rifle.b3d", &btVector3(worldModel->getAbsolutePosition().X, worldModel->getAbsolutePosition().Y, worldModel->getAbsolutePosition().Z), 10, false);

		printf("drop");
	}

	void zoom(scene::ICameraSceneNode * cam) {
		//printf("zoom");
		if (stats.zoom)
			cam->setFOV(stats.zoomFov);
	}

	void destroy() {
		if (model != NULL)
			model->remove();
	}
};

class Revolver : public Weapon {

public:

	Revolver(Scene * sceneIn, core::vector3df location) {

		setStats();

		scene = sceneIn;

		worldModel = getModel("../Models/revolver.b3d");
		worldModel->setScale(core::vector3df(1.0f,1.0f,1.0f));
		worldModel->setPosition(location);

		scene->preloadMesh("../Models/revolverfps.b3d");

		//model = NULL;
	}

	Revolver(Scene * sceneIn, scene::IAnimatedMeshSceneNode * playerModel) {

		setStats();

		scene = sceneIn;		

		worldModel = getModel("../Models/revolver.b3d");
		worldModel->setScale(core::vector3df(1.0f,1.0f,1.0f));
		//worldModel->setPosition(core::vector3df(0.0f,10.0f,10.0f));

		scene->preloadMesh("../Models/revolverfps.b3d");

		activate(playerModel);

		//model = NULL;
	}

	void setStats() {
		//Would like to read these from a file for each weapon
		stats.ammo = 6;
		stats.magazines = 3;
		stats.rate = 500;
		stats.life = 5000;
		stats.zoom = true;
		stats.zoomFov = 0.5;
		stats.speed = 5;
		stats.automatic = false;
		stats.damage = 40;

		//Set the amount of ammo for weapon
		currentAmmo = stats.ammo;	

		nextFire = 0;
	}

	void deactivate(scene::IAnimatedMeshSceneNode * playerModel) {
		worldModel->setParent(playerModel->getJointNode("thigh.R"));
		worldModel->setRotation(core::vector3df(210, -90, -200));
		worldModel->setPosition(core::vector3df(0.4,-0.2,0.2));
	}

	void reload() {
		printf("gun reloaded");
		currentAmmo = stats.ammo;
	}

	void view(scene::ICameraSceneNode * cam, gui::IGUIEnvironment* gui) {
		model = getModel("../Models/revolverfps.b3d");
		model->setScale(core::vector3df(1.0f,1.0f,1.0f));
		//model->setRotation(core::vector3df(0,180,0));
		//model->setPosition(core::vector3df(-0.5f,-9.5f,4.0f));

		model->setPosition(core::vector3df(0.0f,-0.3f,0.65f));

		model->setParent(cam);

		model->setLoopMode(false);

		//if (gui != NULL) {
			reticule = scene->getReticule(gui, "../Textures/ret_revolver.png"); 
		//}
	}

	bool fire(scene::ICameraSceneNode * view) {

		//printf("firing revolver");

		if (scene->getTime() >= nextFire && currentAmmo > 0 ) {

			core::vector3df trajectory = view->getTarget() - view->getPosition();
			//core::vector3df trajectory = view->getRotation();

			nextFire = scene->getTime() + stats.rate; //Firing rate in miliseconds

			currentAmmo -= 1;

			//printf("firing\n");

			IBillboardSceneNode * sprite = scene->getSceneManager()->addBillboardSceneNode(0,  core::dimension2d<f32>(20, 20));
			sprite->setMaterialFlag(video::EMF_LIGHTING, false);
			sprite->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			sprite->setMaterialTexture(0, scene->getTexture("../Textures/plasma.png"));

			trajectory.normalize();
			//printf("X %f, Y %f, Z %f\n",trajectory.X,trajectory.Y,trajectory.Z);

			//sprite->setPosition(core::vector3df(cam->getAbsolutePosition().X + trajectory->X * 15, cam->getAbsolutePosition().Y + trajectory->Y * 15, cam->getAbsolutePosition().Z + trajectory->Z * 15));		

			btVector3 * position = new btVector3(view->getAbsolutePosition().X + trajectory.X * 15, view->getAbsolutePosition().Y + trajectory.Y * 15, view->getAbsolutePosition().Z + trajectory.Z * 15);

			//sprite->setRotation(model->getRotation());
			
			scene->lightUp(sprite, video::SColorf(57, 181 , 74, 0));

			//Particle emitter

			ISceneManager * smgr = scene->getSceneManager();		

			//-----------------Trail

			scene::IParticleSystemSceneNode* trail = smgr->addParticleSystemSceneNode(false);

			//trail->setParticleSize(core::dimension2df(10, 10));

			scene::IParticleEmitter* emitter = trail->createBoxEmitter(
				core::aabbox3d<f32>(1,1,1,1,1,1), // emitter size
				core::vector3df(0.0f,0.00f,0.0f),   // initial direction
				10,1); 

			emitter->setMinStartSize(core::dimension2df(15,15));
			emitter->setMaxStartSize(core::dimension2df(15,15));

			emitter->setMaxStartColor(video::SColor(0,255,255,255));
			emitter->setMinStartColor(video::SColor(0,255,255,255));

			emitter->setMinLifeTime(100);
			emitter->setMaxLifeTime(100);

			trail->setEmitter(emitter); // this grabs the emitter
			//emitter->drop(); // so we can drop it here without deleting it

			//scene::IParticleAffector* paf = ps->createFadeOutParticleAffector();
			scene::IParticleAffector* shrink = trail->createScaleParticleAffector(core::dimension2df(-15,-15));

			//ps->addAffector(paf); // same goes for the affector
			trail->addAffector(shrink);
			//paf->drop();
			//shrink->drop();

			//ps->setPosition(core::vector3df(-70,60,40));
			//ps->setScale(core::vector3df(2,2,2));
			trail->setMaterialFlag(video::EMF_LIGHTING, false);
			trail->setMaterialFlag(video::EMF_ZWRITE_ENABLE, true);
			trail->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("../Textures/plasma.png"));
			trail->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

			trail->setParent(sprite);

			//--------------------Impact

			scene::IParticleSystemSceneNode* impact = smgr->addParticleSystemSceneNode(false);

			//impact->setParticlesAreGlobal(true);

			impact->setVisible(false);

			emitter = impact->createBoxEmitter(
				core::aabbox3d<f32>(1,1,1,1,1,1), // emitter size
				core::vector3df(0.0f,0.5f,0.0f));   // initial direction

			emitter->setMaxAngleDegrees(90);

			emitter->setMaxParticlesPerSecond(20);
			emitter->setMinParticlesPerSecond(20);

			emitter->setMinStartSize(core::dimension2df(10,10));
			emitter->setMaxStartSize(core::dimension2df(20,20));

			emitter->setMaxStartColor(video::SColor(0,255,255,255));
			emitter->setMinStartColor(video::SColor(0,255,255,255));

			emitter->setMinLifeTime(50);
			emitter->setMaxLifeTime(100);

			impact->setEmitter(emitter); // this grabs the emitter
			emitter->drop(); // so we can drop it here without deleting it

			impact->setMaterialFlag(video::EMF_LIGHTING, false);
			impact->setMaterialFlag(video::EMF_ZWRITE_ENABLE, true);
			impact->setMaterialTexture(0, smgr->getVideoDriver()->getTexture("../Textures/plasma.png"));
			impact->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

			scene::IParticleAffector* grav = impact->createGravityAffector(core::vector3df(0,-5.0, 0));
			shrink = impact->createScaleParticleAffector(core::dimension2df(-15,-15));

			//ps->addAffector(paf); // same goes for the affector
			impact->addAffector(grav);
			//paf->drop();
			grav->drop();

			//ps->addAffector(paf); // same goes for the affector
			impact->addAffector(shrink);
			//paf->drop();
			shrink->drop();

			//---------------------emitters done
		
			// Projectile object contains a sprite and keeps track of when to destroy it
			scene->addProjectile(new Projectile(sprite, position, trajectory, scene->getTime() + stats.life, impact, stats.speed, stats.damage));
		}

		return stats.automatic;
	}

	void pickUp() {
		printf("pick up");
	}

	void drop() {
		printf("drop");
	}

	void zoom(scene::ICameraSceneNode * cam) {
		//printf("zoom");
		if (stats.zoom)
			cam->setFOV(stats.zoomFov);
	}

	void destroy() {

	}
};

class Editor : public Weapon {

private:

	Weapon * temp;

public:	

	void view(scene::ICameraSceneNode * camIn, gui::IGUIEnvironment* gui) {

	}

	bool fire(scene::ICameraSceneNode * cam) {
		printf("firing");
	}

	void pickUp() {
		printf("pick up");
	}

	void reload() {
		//printf("reload");
	}

	void drop() {
		printf("drop");
	}

	void storeTemp(Weapon *tempIn) {
		temp = tempIn;
	}

	Weapon *getTemp() {
		return temp;
	}

	void zoom(scene::ICameraSceneNode * cam) {

	}

	void destroy() {

	}
};
