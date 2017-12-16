#pragma once
#include <vector>
#include "ShaderCallBack.h"

class Scene {

private:

	struct Wheel {
		btRaycastVehicle * vehicle;
		Entity * entity;
		int index;
		scene::IBoneSceneNode * point;
		//btWheelInfo& wheel;
	};

	std::vector<struct Wheel> Wheels;

	std::vector<btRaycastVehicle*> vehicles;

	EventQueue * eventQ;



	ISceneManager *irrScene;

	core::list<btRigidBody *> Props;
	core::list<btRigidBody *> Vehicles;
	core::list<btRigidBody *> Weapons;
	core::list<Projectile *> Projectiles;
	//core::list<Entity *> Entities;

	btBroadphaseInterface *BroadPhase;
	btCollisionDispatcher *Dispatcher;
	btAxisSweep3* sweepBP;
	btSequentialImpulseConstraintSolver *Solver;
	btDefaultCollisionConfiguration *CollisionConfiguration;

	scene::ISceneCollisionManager* collMan;

	ITimer * irrTimer;

public:

	btDiscreteDynamicsWorld *World;


	video::IVideoDriver *irrDriver;
	IrrlichtDevice * irrDevice;
	Entities * entities;

	void addEvent(void* (*function)(const void*,const void*), int time);

	Scene(IrrlichtDevice * irrDeviceIn);
	void CreateStartScene();
	//function for adding random stuff to scene for testing purposes	
	void testbed();
	// Synchronises the position of irrlicht objects with their calculated positions from bullet

	//Add a player to the scene
	void addPlayer(const core::vector3df position);

	btGhostObject * CreatePhysicsBox(f32 size);

	btGhostObject * CreatePhysicsSphere(f32 size);

	// Create a box rigid body

	s32 newMaterialType1;

	void addVehicle(btRaycastVehicle * vehicle);

	void CreateBox(const btVector3 &TPosition, const core::vector3df &TScale, btScalar TMass);
	// Create a sphere rigid body
	void CreateSphere(const btVector3 &TPosition, btScalar TRadius, btScalar TMass);
	void addProjectile(Projectile * projectile);
	btVector3 getGravity();
	video::ITexture * getTexture(io::path pathname);
	ISceneManager * getSceneManager();
	btCollisionWorld * getCollisionWorld();
	IAnimatedMesh * getMesh(io::path pathName);
	ISceneNode * getRoot();
	void addObject(btRigidBody * RigidBody);
	void lightUp(ISceneNode * parent, video::SColorf col);
	void update(u32 DeltaTime);
	// Runs the physics simulation. Updates every frame to find new positions and rotations of physics bodies.
	// - TDeltaTime tells the simulation how much time has passed since the last frame so the simulation can run independently of the frame rate.
	void UpdatePhysics(u32 TDeltaTime);
	void UpdateRender(btRigidBody *TObject);
	// Version of updaterender for vehicles
	//void UpdateVehicleRender(btRigidBody *TObject);

	bool UpdateProjectile(Projectile *TObject, u32 TDeltaTime);
	// Removes all props from the world
	void ClearProps();
	//Removes all vehicles from the world
	//void ClearVehicles();
	//Removes all weapons from the world
	void ClearWeapons();

	void Remove(btCollisionObject * object);

	gui::IGUIImage * getReticule(gui::IGUIEnvironment* gui, io::path pathName);

	//Returns current time in the game world
	u32 getTime();

	IAnimatedMeshSceneNode * getMeshNode(io::path pathName);

	void preloadMesh(io::path pathName);

	//Vector converters for working between irrlicht and bullet
	btVector3 convertVector(const core::vector3df * input);
	core::vector3df convertVector( const btVector3 * input);
	void destroy();

	Entity * addEntity(io::path pathName, btVector3 * position, btScalar TMass, bool capsule);

	void removeEntity(Entity * entity);

	Entity * getClose(core::vector3df origin, int id, f32 radius);

	//Entity * addEntity(IMesh * mesh, btVector3 * position, btScalar TMass);

	void addWheels(btRaycastVehicle * vehicle, io::path modelPath, io::path texPath, scene::IBoneSceneNode * point);

	void addWheel(btRaycastVehicle * vehicle, int index, io::path modelPath, io::path texPath, scene::IBoneSceneNode * point);
};
