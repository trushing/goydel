#pragma once

#include "controllable.h"

class Spectator : public Controllable{

private:

	btCollisionWorld *World;

	
	core::vector3df offset;

public:

	Spectator(Scene * scene, btVector3 * position) {

		firing = false;

		World = scene->getCollisionWorld();

		body = scene->addEntity("../Models/editor.b3d", position, 100, true);
        		
		body->physics->setGravity(btVector3(0,0,0));
        body->physics->setDamping(0.95f,0.95f);

		//Set friction so player doesnt slide around so much
		body->physics->setFriction(2.0f);


	}

	void update(f32 delta, bool w, bool a, bool s, bool d, bool shift, f32 * Theta, f32 * Phi) {

		if (*Phi < 1.f)
			*Phi = 2.f;
		else if (179.f < *Phi)
					*Phi = 178.f;

		if (*Theta < 0.f)
			*Theta = 360.f;
		else if (360.f < *Theta)
			*Theta = 0.f;

		f32 sinOfPhi = sinf(*Phi * core::DEGTORAD);
		f32 cosOfPhi = cosf(*Phi * core::DEGTORAD);

		f32 sinOfTheta = sinf(*Theta * core::DEGTORAD);
		f32 cosOfTheta = cosf(*Theta * core::DEGTORAD);

		offset.X = sinOfTheta * sinOfPhi;
		offset.Y = cosOfPhi;
		offset.Z = cosOfTheta * sinOfPhi;

		//cam->setPosition(core::vector3df(node->getAbsolutePosition().X, node->getAbsolutePosition().Y, node->getAbsolutePosition().Z));
		
		body->render->setRotation(core::vector3df(0, cam->getRotation().Y ,0));

		
		if (w || a || s || d) {

			f32 MoveSpeed = 0.16f;

			if(shift)
			{
				MoveSpeed*=1.5;
				//weapon->setRotation(core::vector3df(30,-40,0));
			}

			localForward.X =0;
			localForward.Y =0;
			localForward.Z =0;

			if(w) 
			{
                
				localForward.Z = MoveSpeed;

				//else
					//weapon->setRotation(core::vector3df(0,0,0));*//*
			}

			if(s)
				localForward.Z = -MoveSpeed;
    

			if(a)
				localForward.X = -MoveSpeed;

			if(d)
				localForward.X = MoveSpeed;

			cam->getViewMatrix().inverseRotateVect(localForward);	
			trajectory.setX(localForward.X*2000);
			trajectory.setY(localForward.Y*2000);
			trajectory.setZ(localForward.Z*2000);
			body->physics->setLinearVelocity(trajectory);

		}

		cam->setPosition(core::vector3df(body->physics->getCenterOfMassPosition().getX(), body->physics->getCenterOfMassPosition().getY(), body->physics->getCenterOfMassPosition().getZ()));
		cam->setTarget(core::vector3df(cam->getAbsolutePosition().X + offset.X, cam->getAbsolutePosition().Y + offset.Y,cam->getAbsolutePosition().Z + offset.Z));
		
		//cam->setPosition(core::vector3df(body->physics->getCenterOfMassPosition().getX(), body->physics->getCenterOfMassPosition().getY(), body->physics->getCenterOfMassPosition().getZ()));
		//bodyModel->setPosition(pos);
		//playerNode->setPosition(pos);

	}

	void destroy() {
		body->remove();
		delete this;
	}

	bool align() {
		return true;
	}

	void jump() {
		printf("jump");
	}

	void zoom() {
		printf("zoom");
	}

	Controllable * use(Entity * entity) {

	}

	void activate(scene::ICameraSceneNode * camIn, gui::IGUIEnvironment* guiIn, Controllable * character) {
		gui = guiIn;
		cam = camIn;
		//camIn->setParent(node);

		if (gui != NULL) {
			//playerModel->setVisible(false);
		}

	}

	void deactivate() {
		destroy();
		//scene-
		//World->removeCollisionObject(body->physics);
		

	}

	Controllable * changeView(Controllable * newVessel) {
		return newVessel;
	}

	void reload() {
		//printf("reload");
	}

	void crouch(bool activate) {
		printf("crouch");
	}

	void switchWep() {

	}

	void roll() {

	}

	Controllable * use() {
		return NULL;
	}

};

class Character : public Controllable {

private:

	IBillboardSceneNode * sprite;

	scene::IBoneSceneNode * headBone;
	
	btGhostObject *pheadbox;
	
	Scene * scene;

	ISceneNode * Node;	

	Weapon *w_active;
	Weapon *w_secondary;

	//For calculating rotation speed
	//f32 prevRotate;

	f32 cameraOffset;
		
	core::vector3df offset;

public:

	Character(Scene * sceneIn, btVector3 *position) {
		
		scene = sceneIn;

		sprite = NULL;

		cameraOffset = 6;
		
		firing = false;
		
		gui = NULL;
		cam = NULL;
		
		w_active = NULL;
		w_secondary = NULL;	

		//prevRotate = 0;

		body = scene->addEntity("../Models/body.b3d", position, 100, true);
		
		//body->render->setDebugDataVisible( EDS_SKELETON );

		body->setHealth(100);

		//headBox = sceneIn->getSceneManager()->addCubeSceneNode(10.0f);
		pheadbox = sceneIn->CreatePhysicsBox(7);
		pheadbox->setUserPointer(body);
		pheadbox->setUserIndex(1);
		pheadbox->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

		body->physics->setAngularFactor(btVector3(0,0,0));

		body->physics->setDamping(0,0);

		giveWep(new Revolver(scene, body->render)); 
		giveWep(new Rifle(scene, body->render));

		localForward = core::vector3df(0,0,0);
		trajectory = btVector3(0,0,0);

		body->render->setAnimationSpeed(0);

		headBone = body->render->getJointNode("head");

		node = body->node;

		body->setMaterial("../Textures/alienMap.png");

	}

	void jump() {
		//printf("jump");
		if (isOnGround()) {
			body->physics->applyCentralImpulse(btVector3(0,15000.0f,0));
		}
	}

	bool isOnGround() {    
		//Bullet ray
		//btVector3 start = btVector3(cam->getAbsolutePosition().X,cam->getAbsolutePosition().Y,cam->getAbsolutePosition().Z);
		btVector3 start = body->physics->getCenterOfMassPosition();
		btVector3 end( start.x(), -100.0f,  start.z());

		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);
		//RayCallback.m_collisionFilterGroup = 1;
		//RayCallback.m_collisionFilterMask = 1;
		scene->getCollisionWorld()->rayTest(start, end, RayCallback);

		//RayCallback.
    
		if (RayCallback.hasHit() && start.distance(RayCallback.m_hitPointWorld) < 25.0f) {
			return true;
		} else {
			return false;
		}
	}

	Controllable * use() {

		Controllable * controllable = NULL;

		Entity * target = scene->getClose(body->node->getAbsolutePosition(), 2, 150);

		if (target != NULL)			
			controllable = static_cast <Controllable *> (target->getPointer());
//
		return controllable;
	}

	void update(f32 delta, bool w, bool a, bool s, bool d, bool shift, f32 * Theta, f32 * Phi) {

		if (firing)
			firing = w_active->fire(cam);

		if (*Phi < 1.f)
			*Phi = 2.f;
		else if (179.f < *Phi)
					*Phi = 178.f;

		if (*Theta < 0.f)
			*Theta = 360.f;
		else if (360.f < *Theta)
			*Theta = 0.f;

		f32 sinOfPhi = sinf(*Phi * core::DEGTORAD);
		f32 cosOfPhi = cosf(*Phi * core::DEGTORAD);

		f32 sinOfTheta = sinf(*Theta * core::DEGTORAD);
		f32 cosOfTheta = cosf(*Theta * core::DEGTORAD);

		offset.X = sinOfTheta * sinOfPhi;
		offset.Y = cosOfPhi;
		offset.Z = cosOfTheta * sinOfPhi;

		cam->setPosition(core::vector3df(node->getPosition().X, node->getPosition().Y + cameraOffset, node->getPosition().Z));
		body->render->setRotation(core::vector3df(0, cam->getRotation().Y ,0));

		cam->updateAbsolutePosition();

		cam->setTarget(core::vector3df(cam->getAbsolutePosition().X + offset.X, cam->getAbsolutePosition().Y + offset.Y,cam->getAbsolutePosition().Z + offset.Z));
		
		printf("", body);

		if (gui == NULL) {
			btTransform Transform;
			Transform.setIdentity();
			Transform.setOrigin(btVector3(headBone->getPosition().X, headBone->getPosition().Y, headBone->getPosition().Z + 5));		
			pheadbox->setWorldTransform(Transform);
		}
		f32 rotate = *Theta;

		if (w || a || s || d) {

			btVector3 Bvel = body->physics->getVelocityInLocalPoint(btVector3(0,0,0));
			core::vector3df vel = core::vector3df(Bvel.x(), Bvel.y(), Bvel.z()); 			
			vel.rotateXZBy(rotate);
			
			bool onGround = isOnGround();

			f32 MoveSpeed = 50;

			if(shift && onGround)
			{
				MoveSpeed*=0.5f;
			}

			localForward.Y =0;

			if (w || s) {
				if(w) {
					vel.Z  = MoveSpeed;
				}
				if(s) {			
					vel.Z = -MoveSpeed;
				}
			}

			if (a || d) {
				if(a) {
					vel.X = -MoveSpeed;
				}
				if(d) {
					vel.X = MoveSpeed;
				}
			}
			
			vel.rotateXZBy(-rotate);

			if (onGround) {
				trajectory.setX(vel.X);
				trajectory.setY(vel.Y);
				trajectory.setZ(vel.Z);
				body->physics->setLinearVelocity(trajectory);

			} else {			
				trajectory.setX(vel.X*50);
				trajectory.setY(vel.Y);
				trajectory.setZ(vel.Z*50);
				body->physics->applyCentralForce(trajectory);
			}
		}
	}

	void destroy() {

		//Need code here to drop weapons (w_active and w_secondary)
		w_active->destroy();

		if (gui == NULL)
			scene->Remove(pheadbox);

		delete body;
		
		printf("\n-\nPLAYER IS KILL\n-\n");

		delete this;

	}

	void giveWep( Weapon *weapon) {
		if (w_active != NULL) {

			w_secondary = weapon;
			switchWep();
			//w_active->deactivate(playerModel);
			//printf("deactivating");
		} else {
			w_active = weapon;
			weapon->activate(body->render);

			if (gui != NULL)
				w_active->view(cam, gui);
		}
			
		//w_active = weapon;
		//weapon->activate(playerModel);
		//weapon->parentCam(cam);
	}

	float distanceTo(ISceneNode *node) {
		btVector3 pos = body->physics->getCenterOfMassPosition();
		return node->getAbsolutePosition().getDistanceFrom(core::vector3df(pos.x(), pos.y(),pos.z()));
	}

	btVector3 getVelocity() {
		return body->physics->getLinearVelocity();
	}

	void setVelocity(btVector3 &velocity) {
		body->physics->setLinearVelocity(velocity);
	}

	core::vector3df getPosition() {
		btVector3 pos = body->physics->getCenterOfMassPosition();
		return core::vector3df(pos.x(), pos.y(),pos.z());
	}

	void zoom() {
		//printf("zoom");
		w_active->zoom(cam);
	}

	void activate(scene::ICameraSceneNode * camIn , gui::IGUIEnvironment* guiIn, Controllable * character) {
		
		gui = guiIn;
		cam = camIn;
		//camIn->setParent(node);

		if (gui != NULL) {
			scene->Remove(pheadbox);
			pheadbox = NULL;
			body->render->setVisible(false);
			w_active->view(camIn, gui);
		}	
		
		body->setAutoClean(false);
	}

	void deactivate() {
		w_active->endView(cam, gui);
		body->render->setVisible(true);

		cam = NULL;
		gui = NULL;

		//headBox = scene->getSceneManager()->addCubeSceneNode(10.0f);
		pheadbox = scene->CreatePhysicsBox(7);
		pheadbox->setUserPointer(body);
		pheadbox->setUserIndex(1);
		pheadbox->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

		if (body->node != NULL) {
			body->setAutoClean(true);
		} else {
			delete body;
			delete this;
		}
	}

	Controllable * changeView(Controllable * newVessel) {
		return newVessel;
	}

	void switchWep() {

		if (w_secondary != NULL && w_active != NULL) {
			
			Weapon * temp;

			if (gui != NULL)
				w_active->endView(cam, gui);

			w_active->deactivate(body->render);

			temp = w_active;

			w_active = w_secondary;

			w_secondary = temp;

			w_active->activate(body->render);

			if (gui != NULL)
				w_active->view(cam, gui);

		}
		
	}

	void reload() {
		w_active->reload();
	}

	void crouch(bool activate) {
		if (activate) {
			cameraOffset = 2;
		} else {
			cameraOffset = 10;
		}
	}

	void roll() {
		//printf("roll");

		/*if (cameraOffset > 0) {
			cameraOffset = 0;
		} else {
			cameraOffset = 100;
		}*/
	}
};

class Car : public Controllable {
private:

	
	f32 Radius;

	Scene * scene;
	
	core::vector3df offset;

	scene::ISceneNode * crane;

	btRaycastVehicle * vehicle;
	btDefaultVehicleRaycaster * raycaster;

	btRaycastVehicle::btVehicleTuning tuning;

	Entity * wheel0;
	Entity * wheel1;
	Entity * wheel2;
	Entity * wheel3;
	Entity * wheel4;
	Entity * wheel5;

	btScalar m[16];

	btVector3 * localforward;

	Controllable * driver;

	float	steering;

public:

	Car(Scene * sceneIn, btVector3 *position) {	

		Radius = 100.f;
		
		firing = false;
		
		gui = NULL;
		cam = NULL;
		driver = NULL;

		scene = sceneIn;

		body = scene->addEntity("../Models/chassissuspension.b3d", position, 1000, false);
		body->setMaterial("../Textures/car.png");
		body->render->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
		body->setPointer(this);

		//body->render->setDebugDataVisible( EDS_SKELETON );

		body->render->setJointMode(scene::EJUOR_CONTROL);

		printf("\nJoint count: %u", body->render->getJointCount());
		printf("\n");

		body->makeConvex();

		body->setHealth(100);

		node = body->node;

		raycaster = new btDefaultVehicleRaycaster(scene->World);

		vehicle = new btRaycastVehicle(tuning, body->physics, raycaster);
		
		scene->addVehicle(vehicle);
		
		float connectionHeight = -4.0f;
	
		bool isFrontWheel=true;

		int rightIndex = 0;
		int upIndex = 1;
		int forwardIndex = 2;
		btVector3 wheelDirectionCS0(0.0,-1.0,0.0);
		btVector3 wheelAxleCS(-1.0,0.0,0.0);

		//choose coordinate system
		vehicle->setCoordinateSystem(rightIndex,upIndex,forwardIndex);

		int width = 20;
		int length = 38;

		const int maxProxies = 32766;
		const int maxOverlap = 65535;

		///btRaycastVehicle is the interface for the constraint that implements the raycast vehicle
		///notice that for higher-quality slow-moving vehicles, another approach might be better
		///implementing explicit hinged-wheel constraints with cylinder collision, rather then raycasts
		float	gEngineForce = 0.f;
		float	gBreakingForce = 0.f;

		float	maxEngineForce = 1000.f;//this should be engine/velocity dependent
		float	maxBreakingForce = 100.f;

		steering = 0.f;
		float	steeringIncrement = 0.04f;
		float	steeringClamp = 0.3f;
		float	wheelRadius = 13.0f;
		float	wheelWidth = 5.0f;
		float	wheelFriction = 50;
		float	suspensionStiffness = 40.f;
		float	suspensionDamping = 2.3f;
		float	suspensionCompression = 2.4f;
		float	rollInfluence = 0.01f;
		float suspensionForce = 200000;
		float suspensionTravel = 400;

		float backWheelGap = 25;

		float lengthShift = 2;		

		btScalar suspensionRestLength(7);
		
		btVector3 connectionPointCS0(-width-wheelWidth,connectionHeight,length+(0.3*wheelRadius)  + lengthShift);
		vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,tuning,isFrontWheel);

		connectionPointCS0 = btVector3(width+wheelWidth,connectionHeight,length+(0.3*wheelRadius) + lengthShift);
		vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,tuning,isFrontWheel);

		isFrontWheel = false;

		connectionPointCS0 = btVector3(-width-wheelWidth,connectionHeight,-length-(0.3*wheelRadius)  + lengthShift);
		vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,tuning,isFrontWheel);	

		connectionPointCS0 = btVector3(width+wheelWidth,connectionHeight,-length-(0.3*wheelRadius)  + lengthShift);
		vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,tuning,isFrontWheel);

		connectionPointCS0 = btVector3(-width-wheelWidth,connectionHeight,-length-(0.3*wheelRadius) + backWheelGap  + lengthShift);
		vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,tuning,isFrontWheel);	

		connectionPointCS0 = btVector3(width+wheelWidth,connectionHeight,-length-(0.3*wheelRadius) + backWheelGap  + lengthShift);
		vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,tuning,isFrontWheel);

		
		
		for (int i=0;i<vehicle->getNumWheels();i++)
		{
			btWheelInfo& wheel = vehicle->getWheelInfo(i);
			wheel.m_suspensionStiffness = suspensionStiffness;
			wheel.m_wheelsDampingRelaxation = suspensionDamping;
			wheel.m_wheelsDampingCompression = suspensionCompression;
			wheel.m_frictionSlip = wheelFriction;
			wheel.m_rollInfluence = rollInfluence;
			wheel.m_maxSuspensionForce = suspensionForce;
			wheel.m_maxSuspensionTravelCm = suspensionTravel;
		}

		//scene->addWheels(vehicle, "../Models/wheel.b3d", "../Textures/wheel.png");

		scene->addWheel(vehicle, 0, "../Models/wheel.b3d", "../Textures/wheel.png", NULL);
		scene->addWheel(vehicle, 1, "../Models/wheel.b3d", "../Textures/wheel.png", NULL);
		scene->addWheel(vehicle, 2, "../Models/wheel.b3d", "../Textures/wheel.png", body->render->getJointNode("back-left"));
		scene->addWheel(vehicle, 3, "../Models/wheel.b3d", "../Textures/wheel.png", body->render->getJointNode("back-right"));
		scene->addWheel(vehicle, 4, "../Models/wheel.b3d", "../Textures/wheel.png", body->render->getJointNode("mid-left"));
		scene->addWheel(vehicle, 5, "../Models/wheel.b3d", "../Textures/wheel.png", body->render->getJointNode("mid-right"));
		
		/*wheel0 = scene->addEntity("../Models/wheel.b3d", &vehicle->getWheelInfo(0).m_worldTransform.getOrigin(), 0, false);
		wheel0->setMaterial("../Textures/brushed.jpg");
		wheel1 = scene->addEntity("../Models/wheel.b3d", &vehicle->getWheelInfo(1).m_worldTransform.getOrigin(), 0, false);
		wheel1->setMaterial("../Textures/brushed.jpg");
		wheel2 = scene->addEntity("../Models/wheel.b3d", &vehicle->getWheelInfo(2).m_worldTransform.getOrigin(), 0, false);
		wheel2->setMaterial("../Textures/brushed.jpg");
		wheel3 = scene->addEntity("../Models/wheel.b3d", &vehicle->getWheelInfo(3).m_worldTransform.getOrigin(), 0, false);
		wheel3->setMaterial("../Textures/brushed.jpg");
		wheel4 = scene->addEntity("../Models/wheel.b3d", &vehicle->getWheelInfo(4).m_worldTransform.getOrigin(), 0, false);
		wheel4->setMaterial("../Textures/brushed.jpg");
		wheel5 = scene->addEntity("../Models/wheel.b3d", &vehicle->getWheelInfo(5).m_worldTransform.getOrigin(), 0, false);
		wheel5->setMaterial("../Textures/brushed.jpg");
		
		wheel0->physics->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		wheel1->physics->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		wheel2->physics->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		wheel3->physics->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		wheel4->physics->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		wheel5->physics->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);*/

		steering = 45;
	}

	void jump() {

		vehicle->applyEngineForce(0,0);
		vehicle->applyEngineForce(0,1);
		vehicle->applyEngineForce(0,2);
		vehicle->applyEngineForce(0,3);
		vehicle->applyEngineForce(0,4);
		vehicle->applyEngineForce(0,5);

		vehicle->setBrake(0,0);
		vehicle->setBrake(0,1);
		vehicle->setBrake(0,2);
		vehicle->setBrake(0,3);
		vehicle->setBrake(0,4);
		vehicle->setBrake(0,5);
	}

	void zoom() {

	}

	Controllable * use() {
		return NULL;
	}

	//virtual void rotate(

	void update(f32 delta, bool w, bool a, bool s, bool d, bool shift, f32 * Theta, f32 * Phi) {

		/*btTransform Transform;
		Transform.setIdentity();

		Transform.setRotation(vehicle->getWheelInfo(0).m_worldTransform.getRotation());
		Transform.setOrigin(vehicle->getWheelInfo(0).m_worldTransform.getOrigin());		
		wheel0->physics->setWorldTransform(Transform);

		Transform.setRotation(vehicle->getWheelInfo(1).m_worldTransform.getRotation());
		Transform.setOrigin(vehicle->getWheelInfo(1).m_worldTransform.getOrigin());		
		wheel1->physics->setWorldTransform(Transform);

		Transform.setRotation(vehicle->getWheelInfo(2).m_worldTransform.getRotation());
		Transform.setOrigin(vehicle->getWheelInfo(2).m_worldTransform.getOrigin());		
		wheel2->physics->setWorldTransform(Transform);

		Transform.setRotation(vehicle->getWheelInfo(3).m_worldTransform.getRotation());
		Transform.setOrigin(vehicle->getWheelInfo(3).m_worldTransform.getOrigin());		
		wheel3->physics->setWorldTransform(Transform);

		Transform.setRotation(vehicle->getWheelInfo(4).m_worldTransform.getRotation());
		Transform.setOrigin(vehicle->getWheelInfo(4).m_worldTransform.getOrigin());		
		wheel4->physics->setWorldTransform(Transform);

		Transform.setRotation(vehicle->getWheelInfo(5).m_worldTransform.getRotation());
		Transform.setOrigin(vehicle->getWheelInfo(5).m_worldTransform.getOrigin());		
		wheel5->physics->setWorldTransform(Transform);*/
				
		float MinZoom = 10.f;
			if (Radius < MinZoom)
				Radius = MinZoom;

		if (*Phi < 90.f)
			*Phi = 90.f;
		else if (150.f < *Phi)
					*Phi = 150.f;

		if (*Theta < 0.f)
			*Theta = 360.f;
		else if (360.f < *Theta)
			*Theta = 0.f;

		f32 sinOfPhi = sinf(*Phi * core::DEGTORAD);
		f32 cosOfPhi = cosf(*Phi * core::DEGTORAD);

		f32 sinOfTheta = sinf(*Theta * core::DEGTORAD);
		f32 cosOfTheta = cosf(*Theta * core::DEGTORAD);

		offset.X = Radius * sinOfTheta * sinOfPhi;
		offset.Y = Radius * cosOfPhi;
		offset.Z = Radius * cosOfTheta * sinOfPhi;

		//cam->setPosition(offset);
		cam->setTarget( body->node->getAbsolutePosition() );
		//cam->updateAbsolutePosition();

		//cam->setTarget(node->getAbsolutePosition());

		//cam->setPosition(core::vector3df(body->node->getAbsolutePosition().X + 100, body->node->getAbsolutePosition().Y + 100, body->node->getAbsolutePosition().Z));

		cam->setPosition(core::vector3df(body->node->getAbsolutePosition().X + offset.X, body->node->getAbsolutePosition().Y - offset.Y, body->node->getAbsolutePosition().Z + offset.Z));

		//crane->setPosition(body->node->getAbsolutePosition());


		btVector3 pCam = btVector3(offset.X, body->physics->getCenterOfMassPosition().y(), offset.Z);

		//btVector forward = 

		//f32 direction = atan2(pCam.x(), pCam.z()) / 3.15;

		//f32 angle = ((-vehicle->getForwardVector()).angle(pCam) / 3.15);

		f32 cross = -vehicle->getForwardVector().cross(pCam).y() / 90;
		
		//printf("\n%f", cross );

		//body->physics->getCenterOfMassPosition().angle(

		//steering = *Theta - 180;



		if (!a && !d) {

			steering = 0;

		} else {

			//printf("\n%f", steering);

			if (a) {

				if (steering < -0.60) {
					steering = -0.60;
				} else {
					steering -= 0.01 * delta;
				}
			}

			if (d) {
				if (steering > 0.60) {
					steering = 0.60;
				} else {
					steering += 0.01 * delta;
				}			
			}
		}

//		offset.rotateXZBy(vehicle->getChassisWorldTransform().getRotation().y() * 360);

		//steering = offset.Z / 3.15;
		
		//steering = ((*Theta / 360) - 0.5) - vehicle->getChassisWorldTransform().getRotation().y();		

		//printf("\n%f", cos (body->physics->getOrientation().z() / body->physics->getOrientation().x() ));

		//printf("\n%f", sin(body->physics->getOrientation().y()) * cos( body->physics->getOrientation().x()));


		//printf("\n%f", ((qrot.getY() * qrot.getW())));

		//printf(", %f", -sinOfTheta / 2);

		//steering =  ( -sinOfTheta / 2) - (qrot.getY() * qrot.getW());

		//printf("\n%f", steering);	

		/*btVector3 relative = body->physics->getCenterOfMassPosition();
		relative.setX( relative.getX() - cam->getPosition().X);
		relative.setY( relative.getY() - cam->getPosition().X);
		relative.setZ( relative.getZ() - cam->getPosition().Z);

		printf("\n%f", relative.x());
		printf(", %f", relative.y());
		printf(", %f", relative.z());*/

		core::vector3df eulerRot = body->render->getRotation();

		f32 rot = cam->getRotation().Z;



		//f32 angle = body->physics->getCenterOfMassPosition().angle(direction);
		//printf("\n%f", angle);


		//cam->updateAbsolutePosition();
		//core::vector3df vectorDir = eulerRot.rotationToDirection(cam->getAbsolutePosition());

		//printf("\n%f", cam->getRotation().Y);//.getHorizontalAngle().Y);

		//f32 norm = body->render->getRotation().getSphericalCoordinateAngles().get;

		//offset.rotateXZBy(body->render->getRotation().getHorizontalAngle().Y);
		
		//printf("\n%f", atan2 (offset.X, offset.Z));

		//printf("\n%f", offset.X);

		//steering += 0.001;

		if (w) {
			//body->physics->applyCentralForce(btVector3(-1000000,0,0));
			vehicle->applyEngineForce(30000,0);
			vehicle->applyEngineForce(30000,1);
			vehicle->applyEngineForce(30000,2);
			vehicle->applyEngineForce(30000,3);
			vehicle->applyEngineForce(30000,4);
			vehicle->applyEngineForce(30000,5);
			
		} else {
			if (s) {
				/*vehicle->setBrake(10000000,0);
				vehicle->setBrake(10000000,1);
				vehicle->setBrake(10000000,2);
				vehicle->setBrake(10000000,3);*/

				cross = -cross;

				vehicle->applyEngineForce(-30000,0);
				vehicle->applyEngineForce(-30000,1);
				vehicle->applyEngineForce(-30000,2);
				vehicle->applyEngineForce(-30000,3);
				vehicle->applyEngineForce(-30000,4);
				vehicle->applyEngineForce(-30000,5);
				//body->physics->applyCentralForce(btVector3(1000000,0,0));
			} else {

				vehicle->applyEngineForce(0,0);
				vehicle->applyEngineForce(0,1);
				vehicle->applyEngineForce(0,2);
				vehicle->applyEngineForce(0,3);
				vehicle->applyEngineForce(0,4);
				vehicle->applyEngineForce(0,5);

			}
		}

		steering = cross;

		/*f32 steerDiff = cross - steering;

		if (steerDiff > 0.001) {
			steering =+ delta * 0.00001;
		} else {
			if (steerDiff < -0.001) {
				steering =- delta * 0.00001;
			} else {
				steering = cross;
			}
		}

		if (cross < -0.60) {
			steering = -0.60;
		} else {
			if (cross > 0.60) {
				steering = 0.60;
			}
		}*/

		vehicle->setSteeringValue(steering,0);
		vehicle->setSteeringValue(steering,1);	

		/*vehicle->setSteeringValue(-steering,4);
		vehicle->setSteeringValue(-steering,5);	

		vehicle->setSteeringValue(-steering,2);
		vehicle->setSteeringValue(-steering,3);	*/

		//btTransform Transform;
		//Transform.setIdentity();
		//body->physics->getMotionState()->getWorldTransform(Transform);
		//driver->body->physics->setWorldTransform(Transform);

		/*if(Cursor.X != OldCursor.X)
        {
            Theta += ((AngularVelocity * Elapsed) + (Cursor.X - OldCursor.X) / 2);
        }
        if(Cursor.Y != OldCursor.Y)
        {
            Phi += ((AngularVelocity * Elapsed) + (Cursor.Y - OldCursor.Y) / 2);
        }

        OldCursor = Cursor;*/

		/*if (a) {
			body->physics->applyForce(btVector3(0,0,1000000), btVector3(5, 0, 0));
		} else {
			if (d) {
				body->physics->applyForce(btVector3(0,0,-1000000), btVector3(5, 0, 0));
			}
		}*/
	}

	void destroy() {
		printf("car destroyed");
	}

	void activate(scene::ICameraSceneNode * camIn, gui::IGUIEnvironment* guiIn, Controllable * character) {

		/*x = r * cos(Θ) * sin(Φ);
		y = r * sin(Θ) * sin(Φ);
		z = r * cos(Φ);*/

		gui = guiIn;
		cam = camIn;
		driver = character;
		//driver->body->physics->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
		//cam->setParent(crane);

		//crane->setParent(body->node);

		//cam->setPosition(core::vector3df(100,100,0));

		//cam->setPosition(core::vector3df(-100, 0, 0));

		cam->setInputReceiverEnabled(false);
		cam->bindTargetAndRotation(false);

		//cam->setTarget(core::vector3df(0,0,0));
	}

	void deactivate() {
		cam->bindTargetAndRotation(true);

		gui = NULL;
		cam = NULL;
		//cam->setInputReceiverEnabled(true);		

		vehicle->applyEngineForce(0,0);
		vehicle->applyEngineForce(0,1);
		vehicle->applyEngineForce(0,2);
		vehicle->applyEngineForce(0,3);
		vehicle->applyEngineForce(0,4);
		vehicle->applyEngineForce(0,5);
		
		//river->body->physics->
	}

	Controllable * changeView(Controllable * newVessel) {
		return newVessel;
	}

	void reload() {

	}

	void crouch(bool activate) {

	}

	void switchWep() {

	}

	void roll() {

	}
		
};

class Fighter : public Controllable {
private:

	
	f32 Radius;

	Scene * scene;
	
	core::vector3df offset;

	int lastFire;

	int nextFire;

	Controllable * driver;
	
	btDefaultVehicleRaycaster * raycaster;

	btRaycastVehicle * vehicle;

public:

	Fighter(Scene * sceneIn, btVector3 *position) {	

		nextFire = 0;

		lastFire = 0;

		Radius = 200.f;
		
		firing = false;
		
		gui = NULL;
		cam = NULL;

		scene = sceneIn;

		body = scene->addEntity("../Models/fighter.b3d", position, 5000, false);

		body->setHealth(100);

		body->makeConvex();

		node = body->node;

		body->setPointer(this);

		driver = NULL;

		btRaycastVehicle::btVehicleTuning tuning;

		raycaster = new btDefaultVehicleRaycaster(scene->World);

		vehicle = new btRaycastVehicle(tuning, body->physics, raycaster);		
		
		scene->addVehicle(vehicle);

		//body->physics->setAngularFactor(btVector3(0,1,0));

		//btUprightConstraint *upright = new btUprightConstraint( body, btTransform::getIdentity() );

		//crane = scene->getSceneManager()->addEmptySceneNode();
	}

	void jump() {

	}

	void zoom() {

	}

	void destroy() {

	}

	void fire() {
		int timeGap = lastFire + 200;

		if (scene->getTime() >= timeGap) {

			//printf("%i\n", currentAccuracy);

			//currentAccuracy -= (timeGap / 3);

			//currentAccuracy = std::max(currentAccuracy - (timeGap / 30), stats.accuracy);
			//currentAccuracy = std::max(currentAccuracy, stats.accuracy);

			lastFire = scene->getTime();
			
			//core::vector3df trajectory = body->render->getRotation();

			btVector3 relativeForce = btVector3(2,0,0);
			btTransform boxTrans;
			body->physics->getMotionState()->getWorldTransform(boxTrans);
			btVector3 correctedForce = (boxTrans * relativeForce) - boxTrans.getOrigin();

			core::vector3df trajectory = core::vector3df(correctedForce.getX(), correctedForce.getY(), correctedForce.getZ());

			//trajectory.X += (rand() % currentAccuracy) - (currentAccuracy / 2);
			//trajectory.Y += (rand() % currentAccuracy) - (currentAccuracy / 2);
			//trajectory.Z += (rand() % currentAccuracy) - (currentAccuracy / 2);

			//currentAccuracy += timegap;

			//core::vector3df trajectory = view->getRotation();

			nextFire = scene->getTime() + 200; //Firing rate in miliseconds

			//currentAmmo -= 1;

			//printf("firing\n");

			IBillboardSceneNode * sprite = scene->getSceneManager()->addBillboardSceneNode(0,  core::dimension2d<f32>(20, 20));
			sprite->setMaterialFlag(video::EMF_LIGHTING, false);
			sprite->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			sprite->setMaterialTexture(0, scene->getTexture("../Textures/plasma.png"));

			//trajectory.normalize();
			//printf("X %f, Y %f, Z %f\n",trajectory.X,trajectory.Y,trajectory.Z);

			//sprite->setPosition(core::vector3df(cam->getAbsolutePosition().X + trajectory->X * 15, cam->getAbsolutePosition().Y + trajectory->Y * 15, cam->getAbsolutePosition().Z + trajectory->Z * 15));		

			//btVector3 * position = new btVector3(cam->getAbsolutePosition().X + trajectory.X * 15, cam->getAbsolutePosition().Y + trajectory.Y * 15, cam->getAbsolutePosition().Z + trajectory.Z * 15);
			btVector3 * position = new btVector3(body->render->getPosition().X + trajectory.X * 100, body->render->getPosition().Y + trajectory.Y * 100, body->render->getPosition().Z + trajectory.Z * 100);


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

			sprite->setScale(core::vector3df(5,5,5));

			//---------------------emitters done
		
			// Projectile object contains a sprite and keeps track of when to destroy it
			scene->addProjectile(new Projectile(sprite, position, trajectory, scene->getTime() + 5000, impact, 5, 1000));
		}
	}

	//virtual void rotate(

	void update(f32 delta, bool w, bool a, bool s, bool d, bool shift, f32 * Theta, f32 * Phi) {
		
		btVector3 relativeForce = btVector3(0,0,100);

		if (shift) {
			relativeForce = relativeForce * 10;
		}		 
		
		btTransform boxTrans;
		body->physics->getMotionState()->getWorldTransform(boxTrans);
		btVector3 correctedForce = (boxTrans * relativeForce) - boxTrans.getOrigin();
		body->physics->setLinearVelocity(correctedForce);
		
		//*Theta = ((body->physics->getOrientation().getY() * 180) + 180) + 90;

		//printf("\n%f", abs(body->physics->getOrientation().getY()));

		f32 viewAngle = atan2 (offset.X, offset.Z) / 3.14159265358979323846;

		f32 angleForce = (body->render->getRotation().Y / 360) - 0.5 - viewAngle;
		if (angleForce < -0.5) {
			angleForce = -0.5;
		} else {
			if (angleForce > 0.5) {
				angleForce = 0.5;
			}
		}

		//printf("\n%f", angleForce);

		//printf("\n%f", body->render->getRotation().getHorizontalAngle());

		//body->physics->setAngularVelocity(btVector3(0, ((*Theta / 360) - abs (body->physics->getOrientation().getY())) , 0) );

		//body->physics->setAngularVelocity(btVector3(0, 0.2 , 0) );

		//body->physics->setAngularVelocity(btVector3(0, -angleForce * 2, 0) );

		if (firing)
			fire();
		
		float MinZoom = 10.f;
			if (Radius < MinZoom)
				Radius = MinZoom;

		if (*Phi < 10.f)
			*Phi = 10.f;
		else if (170.f < *Phi)
				*Phi = 170.f;

		if (*Theta < 0.f)
			*Theta = 360.f; 
		else if (360.f < *Theta)
			*Theta = 0.f;

		f32 sinOfPhi = sinf(*Phi * core::DEGTORAD);
		f32 cosOfPhi = cosf(*Phi * core::DEGTORAD);

		f32 sinOfTheta = sinf(*Theta * core::DEGTORAD);
		f32 cosOfTheta = cosf(*Theta * core::DEGTORAD);

		offset.X = Radius * sinOfTheta * sinOfPhi;
		offset.Y = -(Radius * cosOfPhi);
		offset.Z = Radius * cosOfTheta * sinOfPhi;

		//cam->setPosition(offset);
		//cam->setTarget( -offset );
		cam->updateAbsolutePosition();

		cam->setTarget(node->getAbsolutePosition());

		//cam->setPosition(core::vector3df(body->node->getAbsolutePosition().X + 100, body->node->getAbsolutePosition().Y + 100, body->node->getAbsolutePosition().Z));

		cam->setPosition(core::vector3df(body->node->getPosition().X + offset.X, body->node->getPosition().Y + offset.Y, body->node->getPosition().Z + offset.Z));

		//crane->setPosition(body->node->getAbsolutePosition());

		btVector3 pCam = btVector3(offset.X, offset.Y, offset.Z);

		//btVector forward = 

		//f32 direction = atan2(pCam.x(), pCam.z()) / 3.15;

		//f32 angle = ((-vehicle->getForwardVector()).angle(pCam) / 3.15);

		btVector3 cross = vehicle->getForwardVector().cross(pCam) / 90;

		printf("\n%f", cross.y());

		body->physics->setAngularVelocity(btVector3(0,-cross.y() * 10, 0));

		//body->physics->setAngularFactor(btVector3(0, cross.y() * 180, 0));

		//if (w) {
			//body->physics->applyCentralForce(btVector3(-offset.X * 10000,-offset.Y * 10000,-offset.Z* 10000));
		//body->physics->applyCentralForce(btVector3(body->render->getRotation().Z * 1000, body->render->getRotation().Y * 1000, -body->render->getRotation().X *1000) );
		

		/*if (a) {
			body->physics->applyTorque(btVector3(100000000, 0, 0));
		}

		if (d) {
			body->physics->applyTorque(btVector3(-100000000, 0, 0));
		}

		if (w) {
			body->physics->applyTorque(btVector3(0, 0, -100000000));
		}

		if (s) {
			body->physics->applyTorque(btVector3(0, 0, 100000000));
		}*/
		
		//body->physics->applyCentralForce(btVector3(0, 100, 0) );

		//btTransform Transform;
		//Transform.setIdentity();

		//getMotionState()->getWorldTransform(Transform);

		//body->physics->applyCentralForce(body->physics->getOrientation().normalized());;
		//} else {
		//	if (s) {
		//		body->physics->applyCentralForce(btVector3(offset.X * 10000,offset.Y * 10000,offset.Z* 10000));
		//	}
		//}

		/*if(Cursor.X != OldCursor.X)
        {
            Theta += ((AngularVelocity * Elapsed) + (Cursor.X - OldCursor.X) / 2);
        }
        if(Cursor.Y != OldCursor.Y)
        {
            Phi += ((AngularVelocity * Elapsed) + (Cursor.Y - OldCursor.Y) / 2);
        }

        OldCursor = Cursor;*/

		/*if (a) {
			body->physics->applyForce(btVector3(0,0,1000000), btVector3(5, 0, 0));
		} else {
			if (d) {
				body->physics->applyForce(btVector3(0,0,-1000000), btVector3(5, 0, 0));
			}
		}*/

		


		//core::quaternion camRot = core::quaternion(camVec);
		
		//Transform.setRotation(btQuaternion(camRot.X, camRot.Y, camRot.Z, camRot.W));

		//Transform.setRotation(btQuaternion(camRot.X / 3600, camRot.Y / 3600, camRot.Z / 3600, camRot.W / 3600));

		//body->physics->setWorldTransform(Transform);


	}

	void activate(scene::ICameraSceneNode * camIn, gui::IGUIEnvironment* guiIn, Controllable * character) {

		/*x = r * cos(Θ) * sin(Φ);
		y = r * sin(Θ) * sin(Φ);
		z = r * cos(Φ);*/

		gui = guiIn;
		cam = camIn;
		//cam->setParent(crane);

		//crane->setParent(body->node);

		//cam->setPosition(core::vector3df(100,100,0));

		driver = character;

		//cam->setPosition(core::vector3df(-100, 0, 0));

		cam->setInputReceiverEnabled(false);
		cam->bindTargetAndRotation(true);

		body->physics->setGravity(btVector3(0,0,0));

		//cam->setTarget(core::vector3df(0,0,0));
	}

	void deactivate() {

		cam->bindTargetAndRotation(true);


		gui = NULL;
		cam = NULL;
		//cam->setInputReceiverEnabled(true);		

		body->physics->setGravity(scene->getGravity());
	}

	Controllable * changeView(Controllable * newVessel) {
		return newVessel;
	}

	void reload() {

	}

	void crouch(bool activate) {

	}

	void switchWep() {

	}

	void roll() {

	}

	Controllable * use() {
		return NULL;
	}
		
};
