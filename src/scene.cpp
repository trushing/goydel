#include "scene.h"

Scene::Scene(IrrlichtDevice * irrDeviceIn) {

	irrDevice = irrDeviceIn;

	eventQ = new EventQueue();

    // Initialize irrlicht

    //irrGUI = irrDevice->getGUIEnvironment();

    irrScene = irrDevice->getSceneManager();
    irrDriver = irrDevice->getVideoDriver();
    irrDevice->getCursorControl()->setVisible(0);

    // Initialize bullet
    CollisionConfiguration = new btDefaultCollisionConfiguration();
    BroadPhase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000));
    Dispatcher = new btCollisionDispatcher(CollisionConfiguration);

    //btVector3 worldMin(-1000,-1000,-1000);
   // btVector3 worldMax(1000,1000,1000);
    //sweepBP = new btAxisSweep3(worldMin,worldMax);

    Solver = new btSequentialImpulseConstraintSolver();
    World = new btDiscreteDynamicsWorld(Dispatcher, BroadPhase, Solver, CollisionConfiguration);

    //Gravity amount
    World->setGravity(btVector3(0,-150,0));

	irrTimer = irrDevice->getTimer();

	//irrScene->getParameters()->setAttribute(irr::scene::COLLADA_CREATE_SCENE_INSTANCES, true);



    //Creates the world
    CreateStartScene();
}

void Scene::CreateStartScene() {

	irrDriver->setFog(video::SColor(1,226,133,79), video::EFT_FOG_LINEAR, 100, 2000, .0003f, false, true);


	//irrDriver->setFog(video::SColor(1,165,218,244), video::EFT_FOG_LINEAR, 100, 2000, .0003f, false, false);
	//irrDriver->setFog(video::SColor(1, 0,0,0.5),  video::EFT_FOG_LINEAR, 10, 20, 1000, true , true);


    //lighting the scene with a directional light (kind of like a sun)
	float intensity = 1.0f;
    ILightSceneNode * light = irrScene->addLightSceneNode(0, core::vector3df(1000, 1000, 0), video::SColorf(intensity, intensity, intensity, intensity));
    light->enableCastShadow();
    light->setLightType(video::ELT_POINT);
	light->setRadius(1000);
    perror("Skybox Check 1/////////////////");

    //Ambient lighting (shadow darkness and colour)
    intensity = 0.5f;
    video::SColorf ambient = video::SColorf(intensity,intensity,intensity,intensity);
    irrScene->setAmbientLight(ambient);
    irrDriver->setAmbientLight(ambient);

    perror("Skybox Check 2/////////////////");

    //Place ground
    //CreateBox(btVector3(0.0f, 0.0f, 0.0f), core::vector3df(2000.0f, 10.0f, 2000.0f), 0.0f /*0 mass so cant move*/);
    btVector3 myvec = btVector3(0.0f, -200.0f, 0.0f);
	//Entity * ground = addEntity("../Models/basicground.b3d", &myvec, 0, false);

	Entity * ground = new Entity(irrScene, World, "../Models/basicground2.b3d", &myvec, 0, false);
	ground->setMaterial("../Textures/ground.png");

	ground->makeConcave();

	ground->render->getMaterial(0).getTextureMatrix(0).setTextureScale(30, 30);
	ground->render->getMaterial(0).setFlag(video::EMF_FOG_ENABLE, true);

	entities = new Entities(ground);


    perror("Skybox Check");
    //Creates a skybox. Currently using generic stock irrlicht images.
    irrDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    perror("Skybox Check 3/////////////////");

    irrScene->addSkyBoxSceneNode(
	irrDriver->getTexture("../Textures/Skybox2/py.png"),
    irrDriver->getTexture("../Textures/Skybox2/ny.png"),
    irrDriver->getTexture("../Textures/Skybox2/px.png"),
    irrDriver->getTexture("../Textures/Skybox2/nx.png"),
    irrDriver->getTexture("../Textures/Skybox2/nz.png"),
    irrDriver->getTexture("../Textures/Skybox2/pz.png"));
   /* irrDriver->getTexture("../Textures/Skybox/irrlicht2_up.jpg"),
    irrDriver->getTexture("../Textures/Skybox/irrlicht2_dn.jpg"),
    irrDriver->getTexture("../Textures/Skybox/irrlicht2_lf.jpg"),
    irrDriver->getTexture("../Textures/Skybox/irrlicht2_rt.jpg"),
    irrDriver->getTexture("../Textures/Skybox/irrlicht2_ft.jpg"),
    irrDriver->getTexture("../Textures/Skybox/irrlicht2_bk.jpg"));*/
    perror("Skybox Check 4//////////////////");
    irrDriver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
    perror("Skybox Check 5//////////////////");
    testbed();
}

//function for adding random stuff to scene for testing purposes	
void Scene::testbed() {

	Entity * entity = addEntity("../Models/crate.b3d", new btVector3(50 ,50, 0), 200, false);
	entity->setMaterial("../Textures/rust0.jpg");
	entity->setHealth(50);

	Entity * entity2 = addEntity("../Models/crate.b3d", new btVector3(100 ,50, 0), 200, false);
	entity2->setMaterial("../Textures/rust0.jpg");
	entity2->setHealth(50);

	entity2 = addEntity("../Models/crate.b3d", new btVector3(100 ,100, 0), 200, false);
	entity2->setMaterial("../Textures/rust0.jpg");
	entity2->setHealth(50);

	entity2 = addEntity("../Models/billboard.b3d", new btVector3(-400 ,90, -50), 0, false);
	entity2->setMaterial("../Textures/billboard.png");

	entity2->makeConcave();

	/*entity2 = addEntity("../Models/car.b3d", new btVector3(500 ,500, 0), 1000, false);
	entity2->setHealth(50);	*/

	//Player * player = new Player();

    //car = new Vehicle(core::vector3df(100,10,100), irrScene, World, &Vehicles);

    //Spinning shader cubes
    //bool UseHighLevelShaders = true;

    io::path psFileName = "../Shaders/opengl.frag";
    io::path vsFileName = "../Shaders/opengl.vert";

    // Preload textures
    irrDriver->getTexture("../Textures/rock.jpg");
    irrDriver->getTexture("../Textures/brushed.jpg");
    irrDriver->getTexture("../Textures/rust0.jpg");

    //Create materials
    video::IGPUProgrammingServices* gpu = irrDriver->getGPUProgrammingServices();
    newMaterialType1 = 0;

    MyShaderCallBack* mc = new MyShaderCallBack(irrScene);

    newMaterialType1 = gpu->addHighLevelShaderMaterialFromFiles(
            vsFileName, "vertexMain", video::EVST_VS_1_1,
            psFileName, "pixelMain", video::EPST_PS_1_1,
            mc, video::EMT_TRANSPARENT_ADD_COLOR, 0);

    scene::ISceneNode* node = irrScene->addCubeSceneNode(50);
    node->setPosition(core::vector3df(0,100,0));
    node->setMaterialTexture(0, irrDriver->getTexture("../Textures/brushed.jpg"));
    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setMaterialType((video::E_MATERIAL_TYPE)newMaterialType1);

    scene::ISceneNodeAnimator* anim = irrScene->createRotationAnimator(
        core::vector3df(0,0.3f,0));

    node->addAnimator(anim);
    anim->drop();
}

void Scene::addPlayer(core::vector3df position) {

}

void Scene::lightUp(scene::ISceneNode * parent, video::SColorf col) {
	ILightSceneNode * light = irrScene->addLightSceneNode(parent, core::vector3df(0,0,0), col);

    light->enableCastShadow();
    light->setLightType(video::ELT_POINT);

	light->setRadius(1.0f);
}

btGhostObject * Scene::CreatePhysicsSphere(f32 size) {
	btGhostObject * ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(new btSphereShape(size));

	btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(0,0,0));

    ghostObject->setWorldTransform(Transform);

    World->addCollisionObject(ghostObject);

	//ghostPairCallback = new btGhostPairCallback();
	//dynamicsWorld->getPairCache()->setInternalGhostPairCallback(ghostPairCallback);

	return ghostObject;
}

btGhostObject * Scene::CreatePhysicsBox(f32 size) {

	btGhostObject * ghostObject = new btGhostObject();
    ghostObject->setCollisionShape(new btBoxShape(btVector3(size,size,size)));

	btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(0,0,0));

    ghostObject->setWorldTransform(Transform);

    World->addCollisionObject(ghostObject);

	//ghostPairCallback = new btGhostPairCallback();
	//dynamicsWorld->getPairCache()->setInternalGhostPairCallback(ghostPairCallback);

	return ghostObject;


    //World->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	//btCollisionObject* ghostObject;

	/*// Set the initial position of the object
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btVector3(0,0,0));

    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

    // Create the shape
    btVector3 HalfExtents(size, size, size);
    btCollisionShape *Shape = new btBoxShape(HalfExtents);

	// Add mass
    btVector3 LocalInertia;
    Shape->calculateLocalInertia(0, LocalInertia);

    // Create the rigid body object
    //btRigidBody * rigidBody = new btRigidBody(0, MotionState, Shape, LocalInertia);

	//World->addRigidBody(rigidBody);

	//return rigidBody;*/



}

// Create a box rigid body
void Scene::CreateBox(const btVector3 &TPosition, const core::vector3df &TScale, btScalar TMass) {

    ISceneNode *Node = irrScene->addCubeSceneNode(1.0f);
    Node->setScale(TScale);
    Node->setMaterialFlag(video::EMF_LIGHTING, false);
    Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    Node->setMaterialTexture(0, irrDriver->getTexture("../Textures/rust0.jpg"));

    // Set the initial position of the object
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(TPosition);

    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

    // Create the shape
    btVector3 HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
    btCollisionShape *Shape = new btBoxShape(HalfExtents);

    // Add mass
    btVector3 LocalInertia;
    Shape->calculateLocalInertia(TMass, LocalInertia);

    // Create the rigid body object
    btRigidBody *RigidBody = new btRigidBody(TMass, MotionState, Shape, LocalInertia);

    // Store a pointer to the irrlicht node so we can update it later
    RigidBody->setUserPointer((void *)(Node));

    // Add it to the world
    World->addRigidBody(RigidBody);
    Props.push_back(RigidBody);
}

// Create a sphere rigid body
void Scene::CreateSphere(const btVector3 &TPosition, btScalar TRadius, btScalar TMass) {

    ISceneNode *Node = irrScene->addSphereSceneNode(TRadius, 32);
    //Node->setMaterialFlag(video::EMF_LIGHTING, 1);
    Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    Node->setMaterialTexture(0, irrDriver->getTexture("../Textures/brushed.jpg"));

    // Set the initial position of the object
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(TPosition);

    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

    // Create the shape
    btCollisionShape *Shape = new btSphereShape(TRadius);

    // Add mass
    btVector3 LocalInertia;
    Shape->calculateLocalInertia(TMass, LocalInertia);

    // Create the rigid body object
    btRigidBody *RigidBody = new btRigidBody(TMass, MotionState, Shape, LocalInertia);

    // Store a pointer to the irrlicht node so we can update it later
    RigidBody->setUserPointer((void *)(Node));

    // Add it to the world
    World->addRigidBody(RigidBody);
    Props.push_back(RigidBody);
}

void Scene::addProjectile(Projectile * projectile) {
    Projectiles.push_back(projectile);
}

btVector3 Scene::getGravity() {
    return World->getGravity();
}

video::ITexture * Scene::getTexture(io::path pathname) {
    return irrDriver->getTexture(pathname);
}

ISceneManager * Scene::getSceneManager() {
    return irrScene;
}

btCollisionWorld * Scene::getCollisionWorld() {
    return World->getCollisionWorld();
}

IAnimatedMesh * Scene::getMesh(io::path pathName) {
	IAnimatedMesh * node = irrScene->getMesh(pathName);
    return node;
}

IAnimatedMeshSceneNode * Scene::getMeshNode(io::path pathName) {
	IAnimatedMesh * mesh = irrScene->getMesh(pathName);
	IAnimatedMeshSceneNode* model = irrScene->addAnimatedMeshSceneNode( mesh );
	model->setMaterialFlag(video::EMF_LIGHTING, true);
	model->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    return model;

	//ISkinnedMesh *mesh2 =irrScene->createSkinnedMesh();

}

void Scene::preloadMesh(io::path pathName) {
	irrScene->getMesh(pathName);
}

ISceneNode * Scene::getRoot() {
    return irrScene->getRootSceneNode();
}

void Scene::addObject(btRigidBody * RigidBody) {
    World->addRigidBody(RigidBody);
}

void Scene::Remove(btCollisionObject * object) {
	World->removeCollisionObject(object);

	delete object;
}

void Scene::addEvent(void* (*function)(const void*,const void*), int time) {
	eventQ->enqueue(function, time);
}

void Scene::update(u32 DeltaTime) {

    UpdatePhysics(DeltaTime);

	eventQ->update(getTime());



	//effect->update();

    //irrGUI->drawAll();
}

// Runs the physics simulation. Updates every frame to find new positions and rotations of physics bodies.
// - TDeltaTime tells the simulation how much time has passed since the last frame so the simulation can run independently of the frame rate.
void Scene::UpdatePhysics(u32 TDeltaTime)
{

    World->stepSimulation(TDeltaTime * 0.001f, 60);
    core::list<btRigidBody *>::Iterator Iterator;
	core::list<Projectile *>::Iterator ProjectileIterator;
	//core::list<Entity *>::Iterator EntityIterator;
    // Relay the object's orientation to irrlicht
    for(Iterator = Props.begin(); Iterator != Props.end(); ++Iterator)
    {

		//UpdateRender(*Iterator);
		//Inlined for optimization

		ISceneNode *Node = static_cast<ISceneNode *>((*Iterator)->getUserPointer());

		// Set position
		btVector3 Point = (*Iterator)->getCenterOfMassPosition();
		Node->setPosition(core::vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));

		// Set rotation
		core::vector3df Euler;
		const btQuaternion& TQuat = (*Iterator)->getOrientation();
		core::quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
		q.toEuler(Euler);
		Euler *= core::RADTODEG;
		Node->setRotation(Euler);
    }

    for(Iterator = Vehicles.begin(); Iterator != Vehicles.end(); ++Iterator)
    {

        //UpdateVehicleRender(*Iterator);
    }

    //Set up collision management for handling projectile collisions

	u32 counter = 1;

	//TDeltaTime /= 3;

    for(ProjectileIterator = Projectiles.begin(); ProjectileIterator != Projectiles.end();)
    {
        if (!UpdateProjectile(*ProjectileIterator, TDeltaTime))
        {
            ProjectileIterator = Projectiles.erase(ProjectileIterator);
			//printf("removed projectile");
        }
        else
        {
            ProjectileIterator++;
        }
    }

	Entity * entity;

	entity = entities->head;

	int length;

	length = 0;

	//printf("%u\n", length);

	core::vector3df Euler;

	while (entity->next != NULL) {

		entity = entity->next;

		entity->node->setPosition(core::vector3df((entity)->physics->getCenterOfMassPosition().x(), (entity)->physics->getCenterOfMassPosition().y(), (entity)->physics->getCenterOfMassPosition().z()));
		//entity->render->setPosition(core::vector3df(entity->node->getAbsolutePosition().X,entity->node->getAbsolutePosition().Y, entity->node->getAbsolutePosition().Z));
		entity->render->setPosition(core::vector3df(entity->node->getAbsolutePosition().X,entity->node->getAbsolutePosition().Y, entity->node->getAbsolutePosition().Z));

		const btQuaternion& TQuat = (entity)->physics->getOrientation();
		core::quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
		q.toEuler(Euler);
		Euler *= core::RADTODEG;
		entity->node->setRotation(Euler);
		entity->render->setRotation(Euler);

		//if (entity->getId() == 2) {
		//	if (entity->pointer != NULL) {
		//	}
		//}

	}

	btTransform Transform;
	Transform.setIdentity();

	for (int i = 0; i < Wheels.size(); i++) {

		Transform.setRotation(Wheels[i].vehicle->getWheelInfo(Wheels[i].index).m_worldTransform.getRotation());
		Transform.setOrigin(Wheels[i].vehicle->getWheelInfo(Wheels[i].index).m_worldTransform.getOrigin());
		Wheels[i].entity->physics->setWorldTransform(Transform);

		Wheels[i].entity->node->setPosition(core::vector3df(Wheels[i].entity->physics->getCenterOfMassPosition().x(), Wheels[i].entity->physics->getCenterOfMassPosition().y(), Wheels[i].entity->physics->getCenterOfMassPosition().z()));
		Wheels[i].entity->render->setPosition(Wheels[i].entity->node->getPosition());

		const btQuaternion& TQuat = Wheels[i].entity->physics->getOrientation();
		core::quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
		q.toEuler(Euler);
		Euler *= core::RADTODEG;

		Wheels[i].entity->node->setRotation(Euler);
		Wheels[i].entity->render->setRotation(Euler);

		Wheels[i].vehicle->updateWheelTransform(Wheels[i].index,true);

		if (Wheels[i].point != NULL) {
		//	printf("suspension");
			//Wheels[i].point->setPosition(Wheels[i].entity->render->getPosition() - Wheels[i].vehicle->get);

			//Wheels[i].point->updateAbsolutePosition();
			//Wheels[i].point->setPosition(core::vector3df(Wheels[i].vehicle->getWheelInfo(Wheels[i].index).m_chassisConnectionPointCS.getX(),Wheels[i].vehicle->getWheelInfo(Wheels[i].index).m_chassisConnectionPointCS.getY(),Wheels[i].vehicle->getWheelInfo(Wheels[i].index).m_chassisConnectionPointCS..getZ()));

			Wheels[i].point->setPosition(core::vector3df(Wheels[i].vehicle->getWheelInfo(Wheels[i].index).m_chassisConnectionPointCS.getX(),Wheels[i].vehicle->getWheelInfo(Wheels[i].index).m_chassisConnectionPointCS.getY() - Wheels[i].vehicle->getWheelInfo(Wheels[i].index).m_raycastInfo.m_suspensionLength,Wheels[i].vehicle->getWheelInfo(Wheels[i].index).m_chassisConnectionPointCS.getZ()));

			//Wheels[i].vehicle->getWheelInfo(Wheels[i].index).m_raycastInfo.

			//Wheels[i].point->AbsoluteTransformation()
		//} else {
			//printf("nosuspension");
		}

		//printf("lel");

	}

	/*for (int i = 0; i < vehicles.size(); i++) {

		for (int j=0; j < vehicles[i]->getNumWheels();j++)		{
			vehicles[i]->updateWheelTransform(j,false);
		}

	}*/

	//printf("%u\n", length);

	/*for(EntityIterator = Entities.begin(); EntityIterator != Entities.end();++EntityIterator)
    {

		(*EntityIterator)->node->setPosition(core::vector3df((*EntityIterator)->physics->getCenterOfMassPosition().x(), (*EntityIterator)->physics->getCenterOfMassPosition().y(), (*EntityIterator)->physics->getCenterOfMassPosition().z()));
	}*/
}

//UpdateProjectile(*Iterator, ray, collMan, hitPoint, hitPoint, pnt, triangle);

bool Scene::UpdateProjectile(Projectile *TObject, u32 TDeltaTime) {
	Projectile *projectile = static_cast<Projectile *>(TObject);

    // Set position
    //btVector3 Point = TObject->getCenterOfMassPosition();

	if (!projectile->update(irrTimer->getTime(), getCollisionWorld(), TDeltaTime)) {
		//printf("removed projectile");
		//World->removeRigidBody(TObject);
		//printf("removed projectile\n");
		return false;
	} else {
		return true;
	}

}

// Synchronises the position of irrlicht objects with their calculated positions from bullet
void Scene::UpdateRender(btRigidBody *TObject) {
    ISceneNode *Node = static_cast<ISceneNode *>(TObject->getUserPointer());

    // Set position
    btVector3 Point = TObject->getCenterOfMassPosition();
    Node->setPosition(core::vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));

    // Set rotation
    core::vector3df Euler;
    const btQuaternion& TQuat = TObject->getOrientation();
    core::quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
    q.toEuler(Euler);
    Euler *= core::RADTODEG;
    Node->setRotation(Euler);
}

// Version of updaterender for vehicles
/*void Scene::UpdateVehicleRender(btRigidBody *TObject) {
    Vehicle *vehicle = static_cast<Vehicle *>(TObject->getUserPointer());
    ISceneNode *Node = vehicle->getNode();

    //if (Node->getAbsolutePosition().getDistanceFrom(player.getPosition());

    //if (player->distanceTo(Node) < 100) {
        //printf("vehicleInrange");
    //}

    // Set position
    btVector3 Point = TObject->getCenterOfMassPosition();
    Node->setPosition(core::vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));

    // Set rotation
    core::vector3df Euler;
    const btQuaternion& TQuat = TObject->getOrientation();
    core::quaternion q(TQuat.getX(), TQuat.getY(), TQuat.getZ(), TQuat.getW());
    q.toEuler(Euler);
    Euler *= core::RADTODEG;
    Node->setRotation(Euler);
}*/

// Removes all props from the world
void Scene::ClearProps() {

    for(core::list<btRigidBody *>::Iterator Iterator = Props.begin(); Iterator != Props.end(); ++Iterator) {
        btRigidBody *Prop = *Iterator;

        // Delete irrlicht node
        ISceneNode *Node = static_cast<ISceneNode *>(Prop->getUserPointer());
        Node->remove();

        // Remove the object from the world
        World->removeRigidBody(Prop);

        // Free memory
        delete Prop->getMotionState();
        delete Prop->getCollisionShape();
        delete Prop;
    }

    Props.clear();
}

//Removes all vehicles from the world
/*void Scene::ClearVehicles() {

    for(core::list<btRigidBody *>::Iterator Iterator = Vehicles.begin(); Iterator != Vehicles.end(); ++Iterator) {
        btRigidBody *vehicleBody = *Iterator;

        // Delete irrlicht node
        Vehicle *vehicle = static_cast<Vehicle *>(vehicleBody->getUserPointer());
        vehicle->getNode()->remove();

        // Remove the object from the world
        World->removeRigidBody(vehicleBody);

        // Free memory
        delete vehicleBody->getMotionState();
        delete vehicleBody->getCollisionShape();
        delete vehicleBody;
    }

    Props.clear();
}*/

//Removes all weapons from the world
void Scene::ClearWeapons() {

    for(core::list<btRigidBody *>::Iterator Iterator = Weapons.begin(); Iterator != Weapons.end(); ++Iterator) {
        btRigidBody *Weapon = *Iterator;

        // Delete irrlicht node
        ISceneNode *Node = static_cast<ISceneNode *>(Weapon->getUserPointer());
        Node->remove();

        // Remove the object from the world
        World->removeRigidBody(Weapon);

        // Free memory
        delete Weapon->getMotionState();
        delete Weapon->getCollisionShape();
        delete Weapon;
    }

    Weapons.clear();
}

//Vector converters for working between irrlicht and bullet

btVector3 Scene::convertVector(const core::vector3df * input) {
    return btVector3(input->X, input->Y, input->Z);
}

core::vector3df Scene::convertVector(const btVector3 * input) {
    return core::vector3df(input->getX() , input->getY(), input->getZ());
}

u32 Scene::getTime() {
	return irrTimer->getTime();
}

gui::IGUIImage * Scene::getReticule(gui::IGUIEnvironment* gui, io::path pathName) {
	return gui->addImage(getTexture(pathName), core::position2d<int>(irrDriver->getViewPort().getWidth() / 2 - 32,irrDriver->getViewPort().getHeight() / 2 - 32));
}

void Scene::destroy() {
    ClearProps();
    //ClearVehicles();
    ClearWeapons();
    delete World;
    delete Solver;
    delete Dispatcher;
    delete BroadPhase;
    delete CollisionConfiguration;
}

Entity * Scene::addEntity(io::path pathName, btVector3 * position, btScalar TMass, bool capsule) {
	Entity * entity = new Entity(irrScene, World, pathName, position, TMass, capsule);

	//0 mass indicates a static object, so no need to update
	if (TMass !=0)
		entities->add(entity);
	//else
	//	entity->physics->setActivationState(0);
		//Entities.push_back(entity);

	return entity;
}

void Scene::removeEntity(Entity * entity) {

	entity->remove();
}

Entity * Scene::getClose(core::vector3df origin, int id, f32 radius) {

	Entity * target;
	f32 targetDistance;

	target = NULL;

	Entity * entity;

	entity = entities->head;

	while (entity->next != NULL) {

		entity = entity->next;

		if (entity->getPointer() != NULL) {

			f32 distance = origin.getDistanceFrom(entity->node->getAbsolutePosition());

			//target = entity;

			//printf("\n%p", target);

			//printf("\n%f", distance);

			if (distance < radius) {

				printf("\n%p", target);

				if (target == NULL) {
					target = entity;
					targetDistance = distance;
				} else {
					if (targetDistance > distance) {
						target = entity;
						targetDistance = distance;
					}
				}
			}

		}
	}

	return target;
}

void Scene::addVehicle(btRaycastVehicle * vehicle) {
	World->addVehicle(vehicle);
	vehicles.push_back(vehicle);
}

void Scene::addWheels(btRaycastVehicle * vehicle, io::path modelPath, io::path texPath, scene::IBoneSceneNode * point) {

	for (int i = 0; i < vehicle->getNumWheels(); i++) {

		Wheel wheel;

		wheel.vehicle = vehicle;
		wheel.index = i;

		wheel.entity = new Entity(irrScene, World, modelPath, new btVector3(0, 0, 0), 0, false);
		wheel.entity->setMaterial(texPath);

		wheel.entity->physics->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

		wheel.point = point;

		Wheels.push_back(wheel);

	}

	printf("\n%i", Wheels.size());
}

void Scene::addWheel(btRaycastVehicle * vehicle, int index, io::path modelPath, io::path texPath, scene::IBoneSceneNode * point) {

	//for (int i = 0; i < vehicle->getNumWheels(); i++) {

		Wheel wheel;

		wheel.vehicle = vehicle;
		wheel.index = index;

		wheel.entity = new Entity(irrScene, World, modelPath, new btVector3(0, 0, 0), 0, false);
		wheel.entity->setMaterial(texPath);

		wheel.entity->physics->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

		wheel.point = point;
		printf("added wheel");

		Wheels.push_back(wheel);

	//}

	//printf("\n%i", Wheels.size());
}
