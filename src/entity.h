#pragma once
#include "includes.h"

class Entity {

private:

	scene::ISceneManager * irrScene;
	btDiscreteDynamicsWorld *bullScene;

	//If true (default) entity will clean its own memory after removed from scene
	bool autoClean;

public:

	

	btVector3 * mountPoint;

	void * pointer;

	btTriangleMesh* IrrMeshToBullet(scene::IMesh* pMesh,const core::vector3df& scaling);
	
	Entity * prev;
	Entity * next;

	bool dead;

	btRigidBody * physics;
	scene::IAnimatedMeshSceneNode * render;

	scene::ISceneNode * node;

	int health;

	Entity(scene::ISceneManager * irrSceneIn, btDiscreteDynamicsWorld *bullSceneIn, io::path pathName, btVector3 * position, btScalar TMass, bool capsule);

	void setHealth(int newHealth);

	void setAutoClean(bool clean);

	void setId(int newID);

	void mount(btVector3 * point);

	int getId();

	void setPointer(void * newPointer);

	void * getPointer();

	void damage(f32 amount, btVector3 * hitpoint, btVector3 * hitorigin);

	void destroy();

	void setMaterial(io::path pathName);

	void remove();

	void setNext(Entity * entity);

	void setPrev(Entity * entity);

	void makeConcave();

	void makeConvex();

};