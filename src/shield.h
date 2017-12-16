//#include "destructible.h"

/*

class Shield : public Destructible {
private:

	
	
	//btGhostObject * hitSphere;
	//scene::ISceneNode *shieldNode;

	//Scene * scene;

	//scene::ISceneNode * player;

public:

	bool active;

	Shield(scene::ISceneNode * playerIn, Scene * sceneIn) {
		
		//player = playerIn;
		//scene = sceneIn;
		health = 50;

		generate();
		
	}

	void generate() {

		active = true;
		
		//shieldNode = scene->getSceneManager()->addSphereSceneNode(20);

		//shieldNode->setPosition(core::vector3df(3,-10, 5));
		//Node->setScale(core::vector3df(10,10,10));
		//Node->setMaterialFlag(video::EMF_LIGHTING, 1);
		//Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
		//shieldNode->setMaterialTexture(0, scene->getTexture("../Textures/shield.png"));
		//Node->setMaterialTexture(0, scene->getTexture("../Textures/brushed.jpg"));

		//shieldNode->setMaterialFlag(video::EMF_LIGHTING, false);

		//shieldNode->setMaterialType((video::E_MATERIAL_TYPE)scene->newMaterialType1);

		//shieldNode->setMaterialFlag(video::EMF_WIREFRAME , true);
		//Node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL );
		
		//shieldNode->setParent(player);

		//-----------------------------------------collision
		
		//hitSphere = scene->CreatePhysicsBox(20);
		//hitSphere->setUserIndex(3);
		//hitSphere->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

		//hitSphere->setUserPointer(this);
	}

	void update() {

		/*if (shieldNode != NULL) {

			btTransform Transform;
			Transform.setIdentity();

			//Transform.setOrigin(btVector3(5, 5, 5));

			//if (headBone)

			Transform.setOrigin(btVector3(shieldNode->getAbsolutePosition().X, shieldNode->getAbsolutePosition().Y, shieldNode->getAbsolutePosition().Z));
		
			hitSphere->setWorldTransform(Transform);

		}
	}

	void destroy() {

		printf("shield down");

		active = false;
		//scene->Remove(hitSphere);
		//shieldNode->remove();

		//shieldNode = NULL;
	}

	void damage(f32 amount, core::vector3df * origin) {

		if (health > amount) {
			health -= amount;
		} else {
			destroy();
		}

	}
};

*/